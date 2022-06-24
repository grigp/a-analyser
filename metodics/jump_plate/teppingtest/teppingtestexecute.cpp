#include "teppingtestexecute.h"
#include "ui_teppingtestexecute.h"

#include "aanalyserapplication.h"
#include "driver.h"
#include "executewidget.h"
#include "testresultdata.h"
#include "jumpplatedata.h"
#include "settingsprovider.h"
#include "dynamicdiagram.h"

#include <QTimer>
#include <QMessageBox>
#include <QDebug>

TeppingTestExecute::TeppingTestExecute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TeppingTestExecute)
{
    ui->setupUi(this);
    ui->lblCommunicationError->setVisible(false);
    ui->lblRunCount->setVisible(false);
    ui->twPages->setVisible(false);
    QTimer::singleShot(0, this, &TeppingTestExecute::start);
    ui->twPages->setCurrentIndex(0);
}

TeppingTestExecute::~TeppingTestExecute()
{
    delete ui;
}

void TeppingTestExecute::setParams(const QJsonObject &params)
{
    auto sFK = params["finish_kind"].toString();
    m_testFinishKind = JumpPlateDefines::TestFinishKindIndex.value(sFK);
    m_stepsMax = params["steps_count"].toInt();
    m_testTime = params["time"].toInt();

    if (m_testFinishKind == JumpPlateDefines::tfkFixedTime)
        ui->lblCommentFinishTest->setText(QString(tr("Выполняйте шаги на платформе в течение %1 секунд")).arg(m_testTime));
    else
    if (m_testFinishKind == JumpPlateDefines::tfkQuantity)
        ui->lblCommentFinishTest->setText(QString(tr("Выполните %1 шагов на платформе")).arg(m_stepsMax));
    else
    if (m_testFinishKind == JumpPlateDefines::tfkCommand)
        ui->lblCommentFinishTest->setText(tr("Выполняйте шаги на платформе"));
    ui->pbTimeTest->setVisible(m_testFinishKind == JumpPlateDefines::tfkFixedTime);
}

void TeppingTestExecute::closeEvent(QCloseEvent *event)
{
    //! Переехало из деструктора. Подозрение на нерегулярный сбой.
    //! Но для срабатывания необходимо перед delete вызывать close();
    if (m_driver)
    {
        m_driver->stop();
        m_driver->deleteLater();
    }
    QWidget::closeEvent(event);
}

void TeppingTestExecute::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_timerEndOfTest)
    {
        ++m_timeCount;
        ui->pbTimeTest->setValue(100 * m_timeCount / (m_testTime * 50));
        if (m_testFinishKind == JumpPlateDefines::tfkFixedTime &&
                m_timeCount >= m_testTime * 50)
        {
            killTimer(m_timerEndOfTest);
            finishTest();
        }
    }
    else
    if (event->timerId() == m_timerBeforeRun)
    {
        --m_runCounter;
        ui->lblRunCount->setText(QString::number(m_runCounter));
        if (m_runCounter == 0)
        {
            if (m_isRecording)
            {
                m_timerEndOfTest = startTimer(20);
                ui->twPages->setVisible(true);
                ui->lblRunCount->setVisible(false);
            }
        }
    }
}

void TeppingTestExecute::start()
{
    m_driver = static_cast<AAnalyserApplication*>(QApplication::instance())->
            getDriverByProtocols(QStringList() << DeviceProtocols::uid_JumpPlateProtocol);
    if (m_driver)
    {
//        m_jumpControl = dynamic_cast<DeviceProtocols::JumpPlateControl*>(m_driver);
        m_jumpControl = static_cast<DeviceProtocols::JumpPlateControl*>(m_driver->getDeviceControl(DeviceProtocols::uid_JumpPlateControl));

        connect(m_driver, &Driver::sendData, this, &TeppingTestExecute::getData);
        connect(m_driver, &Driver::communicationError, this, &TeppingTestExecute::on_communicationError);

        m_driver->start();

        QTimer::singleShot(300, [=]()
        {
            if (m_jumpControl)
            {
                if (m_jumpControl->platformsCount() != 2)
                {
                    ui->lblCommentFinishTest->setText(tr("Для выполнения теста необходима платформа, состоящая из двух платформ"));
                    ui->btnSave->setVisible(false);
                    ui->lblCommentStateOnPlate->setVisible(false);
                    ui->frIndicator->setVisible(false);
                    ui->pbTimeTest->setVisible(false);
                    ui->frTables->setVisible(false);
                }
                else
                {
                    m_isBlocked = false;
                    m_plt1Pressed = m_jumpControl->platformState(0);
                    m_plt2Pressed = m_jumpControl->platformState(1);
                    iterate(true);
                }
            }
        });

        ui->tvStepsLeft->setModel(&m_mdlLeft);
        ui->tvStepsRight->setModel(&m_mdlRight);
        setModelGeometry();
        setDiagParams();
    }
    else
    {
        QMessageBox::warning(this, tr("Предупреждение"), tr("Отсутствует необходимое подключение для работы теста"));
        static_cast<ExecuteWidget*>(parent())->showDB();
    }
}

void TeppingTestExecute::getData(DeviceProtocols::DeviceData *data)
{
    if (data->uid() == DeviceProtocols::uid_JumpPlateDvcData)
    {
        ui->lblCommunicationError->setVisible(false);

        DeviceProtocols::JumpPlateDvcData *jpData = static_cast<DeviceProtocols::JumpPlateDvcData*>(data);

        if (jpData->plate() == 1)
        {
            m_plt1Pressed = jpData->busy();
            m_plt1Time = jpData->time() / 1000.0;
            iterate(false, BaseDefines::Right);
        }
        else
        if (jpData->plate() == 2)
        {
            m_plt2Pressed = jpData->busy();
            m_plt2Time = jpData->time() / 1000.0;
            iterate(false, BaseDefines::Left);
        }
    }
}

void TeppingTestExecute::on_communicationError(const QString &drvName, const QString &port, const int errorCode)
{
    Q_UNUSED(errorCode);
    ui->lblCommunicationError->setText(QString(tr("Ошибка связи с устройством") + ": %1 (" + tr("порт") + ": %2)").
                                       arg(drvName).arg(port));
    ui->lblCommunicationError->setVisible(true);
}

void TeppingTestExecute::on_recording()
{
    m_isRecording = !m_isRecording;
    if (m_isRecording)
    {
        if (m_testFinishKind == JumpPlateDefines::tfkCommand)
            ui->btnSave->setText(tr("Завершить шаги"));
        else
        {
            ui->btnSave->setIcon(QIcon(":/images/SaveNO.png"));
            ui->btnSave->setText(tr("Прервать шаги"));
        }
        m_runCounter = 3;
        ui->lblRunCount->setVisible(true);
        ui->lblRunCount->setText(QString::number(m_runCounter));
        m_timerBeforeRun = startTimer(1000);
    }
    else
    {
        ui->btnSave->setIcon(QIcon(":/images/Save.png"));
        ui->btnSave->setText(tr("Начать шаги"));
        killTimer(m_timerEndOfTest);
        ui->twPages->setVisible(false);
        ui->lblRunCount->setVisible(false);
        m_runCounter = 3;
    }

    if (!m_isRecording && (m_testFinishKind == JumpPlateDefines::tfkCommand))
        finishTest();
    else
    {
        ui->pbTimeTest->setValue(0);
        m_stepsLeftCount = 0;
        m_stepsRightCount = 0;
        m_timeCount = 0;
        m_time = 0;
        m_mdlLeft.clear();
        m_mdlRight.clear();
        ui->wgtDiagLeftLeg->clear();
        ui->wgtDiagRightLeg->clear();
        setModelGeometry();
    }
}

void TeppingTestExecute::on_selectGraph()
{
    ui->wgtDiagLeftLeg->setKind(DynamicDiagram::KindGraph);
    ui->wgtDiagRightLeg->setKind(DynamicDiagram::KindGraph);
    SettingsProvider::setValueToRegAppCopy("JumpTest", "TeppingTestExecuteDiagKind", static_cast<int>(DynamicDiagram::KindGraph));
}

void TeppingTestExecute::on_selectBar()
{
    ui->wgtDiagLeftLeg->setKind(DynamicDiagram::KindBar);
    ui->wgtDiagRightLeg->setKind(DynamicDiagram::KindBar);
    SettingsProvider::setValueToRegAppCopy("JumpTest", "TeppingTestExecuteDiagKind", static_cast<int>(DynamicDiagram::KindBar));
}

void TeppingTestExecute::on_select3D(bool checked)
{
    if (checked)
    {
        ui->wgtDiagLeftLeg->setVolume(DynamicDiagram::Volume3D);
        ui->wgtDiagRightLeg->setVolume(DynamicDiagram::Volume3D);
        SettingsProvider::setValueToRegAppCopy("JumpTest", "TeppingTestExecuteDiagVolume", static_cast<int>(DynamicDiagram::Volume3D));
    }
    else
    {
        ui->wgtDiagLeftLeg->setVolume(DynamicDiagram::Volume2D);
        ui->wgtDiagRightLeg->setVolume(DynamicDiagram::Volume2D);
        SettingsProvider::setValueToRegAppCopy("JumpTest", "TeppingTestExecuteDiagVolume", static_cast<int>(DynamicDiagram::Volume2D));
    }
}

void TeppingTestExecute::iterate(const bool isStart, BaseDefines::Side side)
{
    if (m_jumpControl->platformsCount() == 2)
    {
        if (m_plt1Pressed)
            ui->frRightLeg->setStyleSheet("background-color:rgb(0, 255, 0)");
        else
            ui->frRightLeg->setStyleSheet("background-color:rgb(255, 255, 255)");
        if (m_plt2Pressed)
            ui->frLeftLeg->setStyleSheet("background-color:rgb(0, 255, 0)");
        else
            ui->frLeftLeg->setStyleSheet("background-color:rgb(255, 255, 255)");

        if (isStart == true)
        {
            ui->lblCommentFinishTest->setVisible(m_plt1Pressed && m_plt2Pressed);
            ui->btnSave->setVisible(m_plt1Pressed && m_plt2Pressed);
            ui->lblCommentStateOnPlate->setVisible(!m_plt1Pressed || !m_plt2Pressed);
        }
        else
        {
            auto addValue = [&](int &stepsCount, double timeContact, double timeNoContact, QStandardItemModel &model,
                                int rc, DynamicDiagram *diag)
            {
                if (timeContact > 0 && timeContact < 2 && timeNoContact > 0 && timeNoContact < 2)
                {
                    ++stepsCount;
                    auto *itemN = new QStandardItem(QString::number(stepsCount));
                    itemN->setEditable(false);
                    itemN->setData(stepsCount, NumberRole);
                    auto *itemContact = new QStandardItem(QString::number(timeContact));
                    itemContact->setEditable(false);
                    itemContact->setData(timeContact, ValueRole);
                    auto *itemNoContact = new QStandardItem(QString::number(timeNoContact));
                    itemNoContact->setEditable(false);
                    itemNoContact->setData(timeNoContact, ValueRole);
                    model.appendRow(QList<QStandardItem*>() << itemN << itemContact << itemNoContact);

                    auto item = new DiagItem(timeNoContact, QString::number(rc));
                    diag->appendItem(item);
                }
            };

            if (!m_isRecording)
            {
                if (m_plt1Pressed && m_plt2Pressed)
                {
                    if (!ui->lblCommentFinishTest->isVisible())
                        ui->lblCommentFinishTest->setVisible(true);
                    if (!ui->btnSave->isVisible())
                        ui->btnSave->setVisible(true);
                    if (ui->lblCommentStateOnPlate->isVisible())
                        ui->lblCommentStateOnPlate->setVisible(false);
                }
                else
                if (!m_plt1Pressed && !m_plt2Pressed)
                {
                    ui->lblCommentFinishTest->setVisible(false);
                    ui->btnSave->setVisible(false);
                    ui->lblCommentStateOnPlate->setVisible(true);
                }
            }
            else
            {
                if (side == BaseDefines::Right)
                {
                    if (m_plt1Pressed)
                        m_plt1TimeNoContact = m_plt1Time;
                    else
                        addValue(m_stepsRightCount, m_plt1Time, m_plt1TimeNoContact, m_mdlRight,
                                 m_mdlRight.rowCount(), ui->wgtDiagRightLeg);
                }
                else
                if (side == BaseDefines::Left)
                {
                    if (m_plt2Pressed)
                        m_plt2TimeNoContact = m_plt2Time;
                    else
                        addValue(m_stepsLeftCount, m_plt2Time, m_plt2TimeNoContact, m_mdlLeft,
                                 m_mdlRight.rowCount(), ui->wgtDiagLeftLeg);
                }

                if (m_testFinishKind == JumpPlateDefines::tfkQuantity &&
                        m_stepsLeftCount + m_stepsRightCount >= m_stepsMax)
                    finishTest();
            }
        }
    }

}

void TeppingTestExecute::setModelGeometry()
{
    m_mdlLeft.setHorizontalHeaderLabels(QStringList() << tr("N") << tr("Время контакта, сек") << tr("Бесконтактная фаза, сек"));
    m_mdlRight.setHorizontalHeaderLabels(QStringList() << tr("N") << tr("Время контакта, сек") << tr("Бесконтактная фаза, сек"));
    ui->tvStepsLeft->header()->resizeSections(QHeaderView::ResizeToContents);
    ui->tvStepsLeft->header()->resizeSection(0, 80);
    ui->tvStepsRight->header()->resizeSections(QHeaderView::ResizeToContents);
    ui->tvStepsRight->header()->resizeSection(0, 80);
}

void TeppingTestExecute::finishTest()
{
    m_isRecording = false;

    auto kard = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentPatient();
    MetodicDefines::MetodicInfo mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentMetodic();

    TestResultData m_trd;  ///< Объект, записывающий данные в базу
    m_trd.newTest(kard.uid, mi.uid);
    m_trd.newProbe(tr("Теппинг тест"));

    auto *data = new TeppingTestData(ChannelsDefines::chanTeppingData);
    data->setTime(static_cast<double>(m_timeCount) / 50);
    for (int i = 0; i < m_mdlLeft.rowCount(); ++i)
    {
        auto tContact = m_mdlLeft.index(i, 1).data(ValueRole).toDouble();
        auto tNoContact = m_mdlLeft.index(i, 2).data(ValueRole).toDouble();
        data->addStep(BaseDefines::Left, tContact, tNoContact);
    }
    for (int i = 0; i < m_mdlRight.rowCount(); ++i)
    {
        auto tContact = m_mdlRight.index(i, 1).data(ValueRole).toDouble();
        auto tNoContact = m_mdlRight.index(i, 2).data(ValueRole).toDouble();
        data->addStep(BaseDefines::Right, tContact, tNoContact);
    }

    m_trd.addChannel(data);

    m_trd.saveTest();
    static_cast<ExecuteWidget*>(parent())->showDB();
}

void TeppingTestExecute::setDiagParams()
{
    ui->wgtDiagLeftLeg->setAxisSpaceBottom(15);
    ui->wgtDiagLeftLeg->setTitleHeight(1);
    ui->wgtDiagRightLeg->setAxisSpaceBottom(15);
    ui->wgtDiagRightLeg->setTitleHeight(1);
    restoreGraphParams();
}

void TeppingTestExecute::restoreGraphParams()
{
    auto kindCode = SettingsProvider::valueFromRegAppCopy("JumpTest", "TeppingTestExecuteDiagKind", 1).toInt();
    DynamicDiagram::Kind kind = static_cast<DynamicDiagram::Kind>(kindCode);
    ui->wgtDiagLeftLeg->setKind(kind);
    ui->wgtDiagRightLeg->setKind(kind);
    ui->btnGraph->setChecked(kind == DynamicDiagram::KindGraph);
    ui->btnBar->setChecked(kind == DynamicDiagram::KindBar);

    auto volumeCode = SettingsProvider::valueFromRegAppCopy("JumpTest", "TeppingTestExecuteDiagVolume", 1).toInt();
    DynamicDiagram::Volume volume = static_cast<DynamicDiagram::Volume>(volumeCode);
    ui->wgtDiagLeftLeg->setVolume(volume);
    ui->wgtDiagRightLeg->setVolume(volume);
    ui->btn3D->setChecked(volume == DynamicDiagram::Volume3D);
}
