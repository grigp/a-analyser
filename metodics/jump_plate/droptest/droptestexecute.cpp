#include "droptestexecute.h"
#include "ui_droptestexecute.h"

#include "aanalyserapplication.h"
#include "driver.h"
#include "executewidget.h"
#include "dataprovider.h"
#include "jumpplatedata.h"
#include "testresultdata.h"
#include "droptestfactors.h"
#include "settingsprovider.h"
#include "droptestdefines.h"
#include "amessagebox.h"

#include <QTimer>
#include <QDebug>

DropTestExecute::DropTestExecute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DropTestExecute)
{
    ui->setupUi(this);
    ui->lblCommunicationError->setVisible(false);
    ui->lblCommentStage->setVisible(false);
    QTimer::singleShot(0, this, &DropTestExecute::start);
    ui->twPages->setCurrentIndex(0);
}

DropTestExecute::~DropTestExecute()
{
    delete ui;
}

void DropTestExecute::setParams(const QJsonObject &params)
{
    auto sFK = params["finish_kind"].toString();
    m_testFinishKind = JumpPlateDefines::TestFinishKindIndex.value(sFK);
    m_jumpsMax = params["jumps_count"].toInt();
    m_testTime = params["time"].toInt();

    if (m_testFinishKind == JumpPlateDefines::tfkFixedTime)
        ui->lblCommentFinishTest->setText(QString(tr("Выполняйте спрыгивания на платформу и прыжки в течение %1 секунд")).arg(m_testTime));
    else
    if (m_testFinishKind == JumpPlateDefines::tfkQuantity)
        ui->lblCommentFinishTest->setText(QString(tr("Выполните %1 спрыгиваний на платформу")).arg(m_jumpsMax));
    else
    if (m_testFinishKind == JumpPlateDefines::tfkCommand)
        ui->lblCommentFinishTest->setText(tr("Выполняйте спрыгивания на платформу"));
    ui->edFallHeight->setValue(params["fall_height"].toDouble());
    ui->pbTimeTest->setVisible(m_testFinishKind == JumpPlateDefines::tfkFixedTime);
}

void DropTestExecute::closeEvent(QCloseEvent *event)
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

void DropTestExecute::timerEvent(QTimerEvent *event)
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
    QWidget::timerEvent(event);
}

void DropTestExecute::start()
{
    m_g = SettingsProvider::valueFromRegAppCopy("UserLocalize", "g", static_cast<QVariant>(9.8)).toDouble();

    m_driver = static_cast<AAnalyserApplication*>(QApplication::instance())->
            getDriverByProtocols(QStringList() << DeviceProtocols::uid_JumpPlateProtocol);
    if (m_driver)
    {
//        m_jumpControl = dynamic_cast<DeviceProtocols::JumpPlateControl*>(m_driver);
        m_jumpControl = static_cast<DeviceProtocols::JumpPlateControl*>(m_driver->getDeviceControl(DeviceProtocols::uid_JumpPlateControl));

        connect(m_driver, &Driver::sendData, this, &DropTestExecute::getData);
        connect(m_driver, &Driver::communicationError, this, &DropTestExecute::on_communicationError);

        m_driver->start();

        m_kard = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentPatient();
        if (m_kard.massa == 0)
        {
            ui->lblCommentFinishTest->setText(tr("Для пациента не указана масса. Проведение теста невозможно."));
            ui->lblCommentGetOffPlate->setVisible(false);
            ui->lblCommentStage->setVisible(false);
            ui->frFallHeight->setVisible(false);
            ui->btnSave->setVisible(false);
            ui->pbTimeTest->setVisible(false);
            ui->tvJumps->setVisible(false);
        }

        QTimer::singleShot(300, [=]()
        {
            if (m_jumpControl && m_kard.massa > 0)
            {
                m_isBlocked = false;
                m_plt1Pressed = m_jumpControl->platformState(0);
                m_plt2Pressed = m_jumpControl->platformState(1);
                iterate(true);
            }
        });

        ui->tvJumps->setModel(&m_mdlTable);
        setModelGeometry();
        setDiagParams();
    }
    else
    {
        AMessageBox::warning(this, tr("Предупреждение"), tr("Отсутствует необходимое подключение для работы теста"));
        static_cast<ExecuteWidget*>(parent())->showDB();
    }
}

void DropTestExecute::getData(DeviceProtocols::DeviceData *data)
{
    if (data->uid() == DeviceProtocols::uid_JumpPlateDvcData)
    {
        ui->lblCommunicationError->setVisible(false);

        DeviceProtocols::JumpPlateDvcData *jpData = static_cast<DeviceProtocols::JumpPlateDvcData*>(data);

        if (jpData->plate() == 1)
        {
            m_plt1Pressed = jpData->busy();
            m_plt1Time = jpData->time() / 1000;
        }
        else
        if (jpData->plate() == 2)
        {
            m_plt2Pressed = jpData->busy();
            m_plt2Time = jpData->time() / 1000;
        }
        iterate(false);
    }
}

void DropTestExecute::on_communicationError(const QString &drvName, const QString &port, const int errorCode)
{
    Q_UNUSED(errorCode);
    ui->lblCommunicationError->setText(QString(tr("Ошибка связи с устройством") + ": %1 (" + tr("порт") + ": %2)").
                                       arg(drvName).arg(port));
    ui->lblCommunicationError->setVisible(true);
}

void DropTestExecute::on_recording()
{
    m_isRecording = !m_isRecording;
    if (m_isRecording)
    {
        if (m_testFinishKind == JumpPlateDefines::tfkCommand)
            ui->btnSave->setText(tr("Завершить прыжки"));
        else
        {
            ui->btnSave->setIcon(QIcon(":/images/SaveNO.png"));
            ui->btnSave->setText(tr("Прервать прыжки"));
        }
        m_timerEndOfTest = startTimer(20);
    }
    else
    {
        ui->btnSave->setIcon(QIcon(":/images/Save.png"));
        ui->btnSave->setText(tr("Начать прыжки"));
        killTimer(m_timerEndOfTest);
    }

    if (!m_isRecording && (m_testFinishKind == JumpPlateDefines::tfkCommand))
        finishTest();
    else
    {
        ui->pbTimeTest->setValue(0);
        m_jumpsCount = 0;
        m_timeCount = 0;
        setStage(dtsWaiting);
        m_mdlTable.clear();
        ui->wgtDiag1->clear();
        ui->wgtDiag2->clear();
        ui->wgtDiag3->clear();
        setModelGeometry();
    }

    ui->cbFactors1->setEnabled(!m_isRecording);
    ui->cbFactors2->setEnabled(!m_isRecording);
    ui->cbFactors3->setEnabled(!m_isRecording);
}

void DropTestExecute::on_selectGraph()
{
    ui->wgtDiag1->setKind(DynamicDiagramDefines::KindGraph);
    ui->wgtDiag2->setKind(DynamicDiagramDefines::KindGraph);
    ui->wgtDiag3->setKind(DynamicDiagramDefines::KindGraph);
    SettingsProvider::setValueToRegAppCopy("JumpTest", "DropTestExecuteDiagKind", static_cast<int>(DynamicDiagramDefines::KindGraph));
}

void DropTestExecute::on_selectBar()
{
    ui->wgtDiag1->setKind(DynamicDiagramDefines::KindBar);
    ui->wgtDiag2->setKind(DynamicDiagramDefines::KindBar);
    ui->wgtDiag3->setKind(DynamicDiagramDefines::KindBar);
    SettingsProvider::setValueToRegAppCopy("JumpTest", "DropTestExecuteDiagKind", static_cast<int>(DynamicDiagramDefines::KindBar));
}

void DropTestExecute::on_select3D(bool checked)
{
    if (checked)
    {
        ui->wgtDiag1->setVolume(DynamicDiagramDefines::Volume3D);
        ui->wgtDiag2->setVolume(DynamicDiagramDefines::Volume3D);
        ui->wgtDiag3->setVolume(DynamicDiagramDefines::Volume3D);
        SettingsProvider::setValueToRegAppCopy("JumpTest", "DropTestExecuteDiagVolume", static_cast<int>(DynamicDiagramDefines::Volume3D));
    }
    else
    {
        ui->wgtDiag1->setVolume(DynamicDiagramDefines::Volume2D);
        ui->wgtDiag2->setVolume(DynamicDiagramDefines::Volume2D);
        ui->wgtDiag3->setVolume(DynamicDiagramDefines::Volume2D);
        SettingsProvider::setValueToRegAppCopy("JumpTest", "DropTestExecuteDiagVolume", static_cast<int>(DynamicDiagramDefines::Volume2D));
    }
}

void DropTestExecute::setDiagParams()
{
    ui->wgtDiag1->setAxisSpaceBottom(15);
    ui->wgtDiag1->setTitleHeight(1);
    ui->wgtDiag2->setAxisSpaceBottom(15);
    ui->wgtDiag2->setTitleHeight(1);
    ui->wgtDiag3->setAxisSpaceBottom(15);
    ui->wgtDiag3->setTitleHeight(1);

    auto factor1 = SettingsProvider::valueFromRegAppCopy("JumpTest", "DropTestFactor1", 5).toInt();
    auto factor2 = SettingsProvider::valueFromRegAppCopy("JumpTest", "DropTestFactor2", 1).toInt();
    auto factor3 = SettingsProvider::valueFromRegAppCopy("JumpTest", "DropTestFactor3", 9).toInt();
    foreach (auto key, DropTestDefines::FactorsByColumn.keys())
    {
        ui->cbFactors1->addItem(DropTestDefines::FactorsByColumn.value(key), key);
        ui->cbFactors2->addItem(DropTestDefines::FactorsByColumn.value(key), key);
        ui->cbFactors3->addItem(DropTestDefines::FactorsByColumn.value(key), key);
    }
    ui->cbFactors1->setCurrentText(DropTestDefines::FactorsByColumn.value(factor1));
    ui->cbFactors2->setCurrentText(DropTestDefines::FactorsByColumn.value(factor2));
    ui->cbFactors3->setCurrentText(DropTestDefines::FactorsByColumn.value(factor3));
    restoreGraphParams();
}

void DropTestExecute::iterate(const bool isStart)
{
    int platformsCount = 1;
    if (m_jumpControl)
        platformsCount = m_jumpControl->platformsCount();

    if (isStart)
    {
        ui->lblCommentFinishTest->setVisible(!m_plt1Pressed);
        ui->btnSave->setVisible(!m_plt1Pressed);
        ui->lblCommentGetOffPlate->setVisible(m_plt1Pressed);
    }
    else
    {
        bool pressed = ((platformsCount == 2 && (m_plt1Pressed || m_plt2Pressed)) ||
                        (platformsCount == 1 && m_plt1Pressed));

        if (!m_isRecording)
        {
            ui->lblCommentFinishTest->setVisible(!pressed);
            ui->btnSave->setVisible(!pressed);
            ui->lblCommentGetOffPlate->setVisible(pressed);
        }
        else
        {
            if (m_stage == dtsWaiting)
            {
                if (pressed)
                {
                    setStage(dtsContact);
                }
            }
            else
            if (m_stage == dtsContact)
            {
                if (!pressed)
                {
                    setStage(dtsJump);
                    m_timeContact = (m_plt1Time + m_plt2Time) / 2.0;
                }
            }
            else
            if (m_stage == dtsJump)
            {
                if (pressed)
                {
                    setStage(dtsResult);
                    m_timeNoContact = (m_plt1Time + m_plt2Time) / 2.0;
                    double height = (m_g * pow(m_timeNoContact, 2)) / 8; // * 100;

                    double power = 0;
                    double stiffness = 0;
                    double initialSpeed = 0;
                    double rsi = 0;
                    DropTestFactors::calculateAdvFactors(m_timeContact, m_timeNoContact, ui->edFallHeight->value(), height, m_kard.massa,
                                                         power, stiffness, initialSpeed, rsi);

                    auto *itemN = new QStandardItem(QString::number(m_jumpsCount + 1));
                    itemN->setEditable(false);
                    itemN->setData(m_jumpsCount, NumberRole);
                    auto *itemTC = new QStandardItem(QString::number(m_timeContact));
                    itemTC->setEditable(false);
                    itemTC->setData(m_timeContact, ValueRole);
                    auto *itemTV = new QStandardItem(QString::number(m_timeNoContact));
                    itemTV->setEditable(false);
                    itemTV->setData(m_timeNoContact, ValueRole);
                    auto *itemM = new QStandardItem(QString::number(m_kard.massa));
                    itemM->setEditable(false);
                    itemM->setData(m_kard.massa, ValueRole);
                    auto *itemFall = new QStandardItem(QString::number(ui->edFallHeight->value()));
                    itemFall->setEditable(false);
                    itemFall->setData(ui->edFallHeight->value(), ValueRole);
                    auto *itemH = new QStandardItem(QString::number(height));
                    itemH->setEditable(false);
                    itemH->setData(height, ValueRole);
                    auto *itemPower = new QStandardItem(QString::number(power));
                    itemPower->setEditable(false);
                    itemPower->setData(power, ValueRole);
                    auto *itemStiff = new QStandardItem(QString::number(stiffness));
                    itemStiff->setEditable(false);
                    itemStiff->setData(stiffness, ValueRole);
                    auto *itemIS = new QStandardItem(QString::number(initialSpeed));
                    itemIS->setEditable(false);
                    itemIS->setData(initialSpeed, ValueRole);
                    auto *itemRSI = new QStandardItem(QString::number(rsi));
                    itemRSI->setEditable(false);
                    itemRSI->setData(rsi, ValueRole);

                    m_mdlTable.appendRow(QList<QStandardItem*>() << itemN << itemTC << itemTV << itemM << itemFall
                                         << itemH << itemPower << itemStiff << itemIS << itemRSI);

                    addItemToDiag();
                    ++m_jumpsCount;
                    if (m_testFinishKind == JumpPlateDefines::tfkQuantity && m_jumpsCount >= m_jumpsMax)
                        finishTest();
                }
            }
            else
            if (m_stage == dtsResult)
            {
                if (!pressed)
                    setStage(dtsWaiting);
            }
        }
    }
}

void DropTestExecute::addItemToDiag()
{
    auto col1 = ui->cbFactors1->currentData().toInt();
    auto v1 = m_mdlTable.index(m_mdlTable.rowCount() - 1, col1).data(ValueRole).toDouble();
    auto item1 = new DiagItem(v1, QString::number(m_mdlTable.rowCount()));
    ui->wgtDiag1->appendItem(item1);

    auto col2 = ui->cbFactors2->currentData().toInt();
    auto v2 = m_mdlTable.index(m_mdlTable.rowCount() - 1, col2).data(ValueRole).toDouble();
    auto item2 = new DiagItem(v2, QString::number(m_mdlTable.rowCount()));
    ui->wgtDiag2->appendItem(item2);

    auto col3 = ui->cbFactors3->currentData().toInt();
    auto v3 = m_mdlTable.index(m_mdlTable.rowCount() - 1, col3).data(ValueRole).toDouble();
    auto item3 = new DiagItem(v3, QString::number(m_mdlTable.rowCount()));
    ui->wgtDiag3->appendItem(item3);
}

void DropTestExecute::setStage(const DropTestExecute::Stage stage)
{
    m_stage = stage;
    switch (m_stage)
    {
    case dtsWaiting:
        ui->lblCommentStage->setText(tr("Выполните прыжок на платформу"));
        break;
    case dtsContact:
        ui->lblCommentStage->setText(tr("Прыгните в высоту максимально"));
        break;
    case dtsJump:
        ui->lblCommentStage->setText(tr("Прыжок"));
        break;
    case dtsResult:
        ui->lblCommentStage->setText(tr("Сойдите с платформы"));
        break;
    default:
        ui->lblCommentStage->setText(tr(""));
        break;
    }
    ui->lblCommentStage->setVisible(true);
}

void DropTestExecute::setModelGeometry()
{
    m_mdlTable.setHorizontalHeaderLabels(QStringList() << tr("N")
                                         << tr("Контактная\nфаза, сек")
                                         << tr("Бесконтактная\nфаза, сек")
                                         << tr("Масса\nпациента, кг")
                                         << tr("Высота\nспрыгивания, м")
                                         << tr("Высота\nпрыжка, м")
                                         << tr("Мощность")
                                         << tr("Жесткость")
                                         << tr("Начальная\nскорость, м/сек")
                                         << tr("Индекс\nреактивной силы"));
    ui->tvJumps->header()->resizeSections(QHeaderView::ResizeToContents);
    ui->tvJumps->header()->resizeSection(0, 100);
}

void DropTestExecute::finishTest()
{
    m_isRecording = false;

    MetodicDefines::MetodicInfo mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentMetodic();

    TestResultData m_trd;  ///< Объект, записывающий данные в базу
    m_trd.newTest(m_kard.uid, mi.uid);
    m_trd.newProbe(tr("Дроп тест"));

    auto *data = new DropTestData(ChannelsDefines::chanDropTestData);
    data->setTime(static_cast<double>(m_timeCount) / 50);
    for (int i = 0; i < m_mdlTable.rowCount(); ++i)
    {
        auto tC = m_mdlTable.index(i, 1).data(ValueRole).toDouble();
        auto tNC = m_mdlTable.index(i, 2).data(ValueRole).toDouble();
        auto fallH = m_mdlTable.index(i, 4).data(ValueRole).toDouble();
        auto h = m_mdlTable.index(i, 5).data(ValueRole).toDouble();
        data->addJump(fallH, h, tNC, tC);
    }

    m_trd.addChannel(data);

    m_trd.saveTest();
    static_cast<ExecuteWidget*>(parent())->showDB();
}

void DropTestExecute::restoreGraphParams()
{
    auto kindCode = SettingsProvider::valueFromRegAppCopy("JumpTest", "DropTestExecuteDiagKind", 1).toInt();
    DynamicDiagramDefines::Kind kind = static_cast<DynamicDiagramDefines::Kind>(kindCode);
    ui->wgtDiag1->setKind(kind);
    ui->wgtDiag2->setKind(kind);
    ui->wgtDiag3->setKind(kind);
    ui->btnGraph->setChecked(kind == DynamicDiagramDefines::KindGraph);
    ui->btnBar->setChecked(kind == DynamicDiagramDefines::KindBar);

    auto volumeCode = SettingsProvider::valueFromRegAppCopy("JumpTest", "DropTestExecuteDiagVolume", 1).toInt();
    DynamicDiagramDefines::Volume volume = static_cast<DynamicDiagramDefines::Volume>(volumeCode);
    ui->wgtDiag1->setVolume(volume);
    ui->wgtDiag2->setVolume(volume);
    ui->wgtDiag3->setVolume(volume);
    ui->btn3D->setChecked(volume == DynamicDiagramDefines::Volume3D);
}
