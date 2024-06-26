#include "jumpheighttestexecute.h"
#include "ui_jumpheighttestexecute.h"

#include "aanalyserapplication.h"
#include "driver.h"
#include "executewidget.h"
#include "testresultdata.h"
#include "jumpplatedata.h"
#include "settingsprovider.h"
#include "amessagebox.h"

#include <QTimer>
#include <QDebug>

JumpHeightTestExecute::JumpHeightTestExecute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JumpHeightTestExecute)
{
    ui->setupUi(this);
    ui->lblCommunicationError->setVisible(false);
    QTimer::singleShot(0, this, &JumpHeightTestExecute::start);
}

JumpHeightTestExecute::~JumpHeightTestExecute()
{
    delete ui;
}

void JumpHeightTestExecute::setParams(const QJsonObject &params)
{
    auto sFK = params["finish_kind"].toString();
    m_testFinishKind = JumpPlateDefines::TestFinishKindIndex.value(sFK);

    m_jumpsMax = params["jumps_count"].toInt();
    m_testTime = params["time"].toInt();

    auto sStr = params["strategy"].toString();
    m_strategy = JumpHeightTestDefines::StrategyIndex.value(sStr);

    m_contactTimeBound = params["contact_time_bound"].toInt();

    if (m_testFinishKind == JumpPlateDefines::tfkFixedTime)
        ui->lblCommentFinishTest->setText(QString(tr("Выполняйте прыжки на платформе в течение %1 секунд")).arg(m_testTime));
    else
    if (m_testFinishKind == JumpPlateDefines::tfkQuantity)
        ui->lblCommentFinishTest->setText(QString(tr("Выполните %1 прыжков на платформе")).arg(m_jumpsMax));
    else
    if (m_testFinishKind == JumpPlateDefines::tfkCommand)
        ui->lblCommentFinishTest->setText(tr("Выполняйте прыжки на платформе"));

    if (m_strategy == JumpHeightTestDefines::jhsMaxHeight)
        ui->lblCommentStrategy->setText(QString(tr("с максимальной высотой прыжка")));
    else
    if (m_strategy == JumpHeightTestDefines::jhsMinContactTime)
        ui->lblCommentStrategy->setText(QString(tr("с минимальным временем контакта с платформой")));
    ui->pbTimeTest->setVisible(m_testFinishKind == JumpPlateDefines::tfkFixedTime);
}

void JumpHeightTestExecute::closeEvent(QCloseEvent *event)
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

void JumpHeightTestExecute::timerEvent(QTimerEvent *event)
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
}

void JumpHeightTestExecute::start()
{
    m_g = SettingsProvider::valueFromRegAppCopy("UserLocalize", "g", static_cast<QVariant>(9.8)).toDouble();

    m_driver = static_cast<AAnalyserApplication*>(QApplication::instance())->
            getDriverByProtocols(QStringList() << DeviceProtocols::uid_JumpPlateProtocol);
    if (m_driver)
    {
//        m_jumpControl = dynamic_cast<DeviceProtocols::JumpPlateControl*>(m_driver);
        m_jumpControl = static_cast<DeviceProtocols::JumpPlateControl*>(m_driver->getDeviceControl(DeviceProtocols::uid_JumpPlateControl));

        connect(m_driver, &Driver::sendData, this, &JumpHeightTestExecute::getData);
        connect(m_driver, &Driver::communicationError, this, &JumpHeightTestExecute::on_communicationError);

        m_driver->start();

        QTimer::singleShot(300, [=]()
        {
            if (m_jumpControl)
            {
                m_isBlocked = false;
                m_plt1Pressed = m_jumpControl->platformState(0);
                m_plt2Pressed = m_jumpControl->platformState(1);
                iterate(true);
            }
        });

        ui->tvJumps->setModel(&m_mdlTable);
        ui->wgtJumpsHeight->setTitle(tr("Высота прыжка, м"));
        ui->wgtTimeContact->setTitle(tr("Время контактной фазы, сек"));
        ui->wgtJumpsHeight->setVolume(DynamicDiagramDefines::Volume3D);
        ui->wgtJumpsHeight->setKind(DynamicDiagramDefines::KindGraph);
        ui->wgtTimeContact->setVolume(DynamicDiagramDefines::Volume3D);
        ui->wgtTimeContact->setKind(DynamicDiagramDefines::KindGraph);
        setModelGeometry();
        restoreGraphParams();
    }
    else
    {
        AMessageBox::warning(this, tr("Предупреждение"), tr("Отсутствует необходимое подключение для работы теста"));
        static_cast<ExecuteWidget*>(parent())->showDB();
    }
}

void JumpHeightTestExecute::getData(DeviceProtocols::DeviceData *data)
{
    if (data->uid() == DeviceProtocols::uid_JumpPlateDvcData)
    {
        ui->lblCommunicationError->setVisible(false);

        DeviceProtocols::JumpPlateDvcData *jpData = static_cast<DeviceProtocols::JumpPlateDvcData*>(data);

        if (jpData->plate() == 1)
        {
            m_plt1Pressed = jpData->busy();
            m_plt1Time = jpData->time() / 1000;
            m_plt1Height = (m_g * pow(jpData->time() / 1000, 2)) / 8; // * 100;
        }
        else
        if (jpData->plate() == 2)
        {
            m_plt2Pressed = jpData->busy();
            m_plt2Time = jpData->time() / 1000;
            m_plt2Height = (m_g * pow(jpData->time() / 1000, 2)) / 8; // * 100;
        }
        iterate(false);
    }
}

void JumpHeightTestExecute::on_communicationError(const QString &drvName, const QString &port, const int errorCode)
{
    Q_UNUSED(errorCode);
    ui->lblCommunicationError->setText(QString(tr("Ошибка связи с устройством") + ": %1 (" + tr("порт") + ": %2)").
                                       arg(drvName).arg(port));
    ui->lblCommunicationError->setVisible(true);
}

void JumpHeightTestExecute::on_recording()
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
        m_height = 0;
        m_time = 0;
        m_mdlTable.clear();
        ui->wgtJumpsHeight->clear();
        ui->wgtTimeContact->clear();
        setModelGeometry();
    }
}

void JumpHeightTestExecute::on_selectGraph()
{
    ui->wgtJumpsHeight->setKind(DynamicDiagramDefines::KindGraph);
    ui->wgtTimeContact->setKind(DynamicDiagramDefines::KindGraph);

    SettingsProvider::setValueToRegAppCopy("JumpTest", "HeightTestExecuteDiagKind", static_cast<int>(DynamicDiagramDefines::KindGraph));
}

void JumpHeightTestExecute::on_selectBar()
{
    ui->wgtJumpsHeight->setKind(DynamicDiagramDefines::KindBar);
    ui->wgtTimeContact->setKind(DynamicDiagramDefines::KindBar);
    SettingsProvider::setValueToRegAppCopy("JumpTest", "HeightTestExecuteDiagKind", static_cast<int>(DynamicDiagramDefines::KindBar));
}

void JumpHeightTestExecute::on_select3D(bool checked)
{
    if (checked)
    {
        ui->wgtJumpsHeight->setVolume(DynamicDiagramDefines::Volume3D);
        ui->wgtTimeContact->setVolume(DynamicDiagramDefines::Volume3D);
        SettingsProvider::setValueToRegAppCopy("JumpTest", "HeightTestExecuteDiagVolume", static_cast<int>(DynamicDiagramDefines::Volume3D));
    }
    else
    {
        ui->wgtJumpsHeight->setVolume(DynamicDiagramDefines::Volume2D);
        ui->wgtTimeContact->setVolume(DynamicDiagramDefines::Volume2D);
        SettingsProvider::setValueToRegAppCopy("JumpTest", "HeightTestExecuteDiagVolume", static_cast<int>(DynamicDiagramDefines::Volume2D));
    }
}

void JumpHeightTestExecute::iterate(const bool isStart)
{
    int platformsCount = 1;
    if (m_jumpControl)
        platformsCount = m_jumpControl->platformsCount();

    if (isStart)
    {
        ui->lblCommentFinishTest->setVisible(m_plt1Pressed);
        ui->lblCommentStrategy->setVisible(m_plt1Pressed);
        ui->btnSave->setVisible(m_plt1Pressed);
        ui->lblCommentStateOnPlate->setVisible(!m_plt1Pressed);
    }
    else
    {
        if ((platformsCount == 2 && m_plt1Pressed && m_plt2Pressed) ||
            (platformsCount == 1 && m_plt1Pressed))
        {
            if (!ui->lblCommentFinishTest->isVisible())
                ui->lblCommentFinishTest->setVisible(true);
            if (!ui->lblCommentStrategy->isVisible())
                ui->lblCommentStrategy->setVisible(true);
            if (!ui->btnSave->isVisible())
                ui->btnSave->setVisible(true);
            if (ui->lblCommentStateOnPlate->isVisible())
                ui->lblCommentStateOnPlate->setVisible(false);
        }

        if (m_isRecording)
        {
            if ((platformsCount == 2 && m_plt1Pressed && m_plt2Pressed) ||
                (platformsCount == 1 && m_plt1Pressed))
            {
                m_height = m_plt1Height;
                if (platformsCount == 2)
                    m_height = (m_plt1Height + m_plt2Height) / 2;
            }
            else
            if ((platformsCount == 2 && !m_plt1Pressed && !m_plt2Pressed) ||
                (platformsCount == 1 && !m_plt1Pressed))
            {
                m_time = m_plt1Time;
                if (platformsCount == 2)
                    m_time = (m_plt1Time + m_plt2Time) / 2;
                if (m_height > 0 && m_height < 2 && m_time > 0) //(m_jumpsCount > 0)
                {
                    auto *itemN = new QStandardItem(QString::number(m_jumpsCount));
                    itemN->setEditable(false);
                    itemN->setData(m_jumpsCount, NumberRole);
                    auto *itemH = new QStandardItem(QString::number(m_height));
                    itemH->setEditable(false);
                    itemH->setData(m_height, ValueRole);
                    auto *itemT = new QStandardItem(QString::number(m_time));
                    itemT->setEditable(false);
                    itemT->setData(m_time, ValueRole);

                    m_mdlTable.appendRow(QList<QStandardItem*>() << itemN << itemH << itemT);

                    auto diH = new DiagItem(m_height, QString::number(m_jumpsCount));
                    ui->wgtJumpsHeight->appendItem(diH);
                    auto diTC = new DiagItem(m_time, QString::number(m_jumpsCount));
                    ui->wgtTimeContact->appendItem(diTC);
                }

                ++m_jumpsCount;
                if (m_testFinishKind == JumpPlateDefines::tfkQuantity && m_jumpsCount > m_jumpsMax)
                    finishTest();
            }
        }
    }
}

void JumpHeightTestExecute::setModelGeometry()
{
    m_mdlTable.setHorizontalHeaderLabels(QStringList() << tr("N") << tr("Высота прыжка, м") << tr("Время контактной фазы, сек"));
    ui->tvJumps->header()->resizeSections(QHeaderView::ResizeToContents);
    ui->tvJumps->header()->resizeSection(0, 100);
}

void JumpHeightTestExecute::finishTest()
{
    m_isRecording = false;

    auto kard = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentPatient();
    MetodicDefines::MetodicInfo mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentMetodic();

    TestResultData m_trd;  ///< Объект, записывающий данные в базу
    m_trd.newTest(kard.uid, mi.uid);
    m_trd.newProbe(tr("Анализ высоты серии прыжков"));

    auto *data = new JumpHeightData(ChannelsDefines::chanJumpHeight);
    data->setTime(static_cast<double>(m_timeCount) / 50);
    for (int i = 0; i < m_mdlTable.rowCount(); ++i)
    {
        auto h = m_mdlTable.index(i, 1).data(ValueRole).toDouble();
        auto t = m_mdlTable.index(i, 2).data(ValueRole).toDouble();
        data->addJump(h, t);
    }

    m_trd.addChannel(data);

    m_trd.saveTest();
    static_cast<ExecuteWidget*>(parent())->showDB();
}

void JumpHeightTestExecute::restoreGraphParams()
{
    auto kindCode = SettingsProvider::valueFromRegAppCopy("JumpTest", "HeightTestExecuteDiagKind", 1).toInt();
    DynamicDiagramDefines::Kind kind = static_cast<DynamicDiagramDefines::Kind>(kindCode);
    ui->wgtJumpsHeight->setKind(kind);
    ui->wgtTimeContact->setKind(kind);
    ui->btnGraph->setChecked(kind == DynamicDiagramDefines::KindGraph);
    ui->btnBar->setChecked(kind == DynamicDiagramDefines::KindBar);

    auto volumeCode = SettingsProvider::valueFromRegAppCopy("JumpTest", "HeightTestExecuteDiagVolume", 1).toInt();
    DynamicDiagramDefines::Volume volume = static_cast<DynamicDiagramDefines::Volume>(volumeCode);
    ui->wgtJumpsHeight->setVolume(volume);
    ui->wgtTimeContact->setVolume(volume);
    ui->btn3D->setChecked(volume == DynamicDiagramDefines::Volume3D);
}
