#include "droptestexecute.h"
#include "ui_droptestexecute.h"

#include "aanalyserapplication.h"
#include "driver.h"
#include "executewidget.h"
#include "dataprovider.h"
#include "jumpplatedata.h"
#include "testresultdata.h"
#include "droptestfactors.h"

#include <QTimer>
#include <QMessageBox>

DropTestExecute::DropTestExecute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DropTestExecute)
{
    ui->setupUi(this);
    ui->lblCommunicationError->setVisible(false);
    ui->lblCommentStage->setVisible(false);
    QTimer::singleShot(0, this, &DropTestExecute::start);
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
    ui->edFallHeight->setValue(params["fall_height"].toInt());
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
    m_driver = static_cast<AAnalyserApplication*>(QApplication::instance())->
            getDriverByProtocols(QStringList() << DeviceProtocols::uid_JumpPlateProtocol);
    if (m_driver)
    {
        m_jumpControl = dynamic_cast<DeviceProtocols::JumpPlateControl*>(m_driver);

        connect(m_driver, &Driver::sendData, this, &DropTestExecute::getData);
        connect(m_driver, &Driver::communicationError, this, &DropTestExecute::on_communicationError);

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
        setModelGeometry();

        m_kard = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedPatient();
    }
    else
    {
        QMessageBox::warning(this, tr("Предупреждение"), tr("Отсутствует необходимое подключение для работы теста"));
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
            //m_plt1Height = (9.8 * pow(jpData->time() / 1000, 2)) / 8 * 100;
        }
        else
        if (jpData->plate() == 2)
        {
            m_plt2Pressed = jpData->busy();
            m_plt2Time = jpData->time() / 1000;
            //m_plt2Height = (9.8 * pow(jpData->time() / 1000, 2)) / 8 * 100;
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
        ui->btnSave->setIcon(QIcon(":/images/SaveNO.png"));
        ui->btnSave->setText(tr("Прервать прыжки"));
        m_timerEndOfTest = startTimer(20);
    }
    else
    {
        ui->btnSave->setIcon(QIcon(":/images/Save.png"));
        ui->btnSave->setText(tr("Начать прыжки"));
        killTimer(m_timerEndOfTest);
    }

    ui->pbTimeTest->setValue(0);
    m_jumpsCount = 0;
    m_timeCount = 0;
    setStage(dtsWaiting);
    m_mdlTable.clear();
    setModelGeometry();
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
                    double height = (9.8 * pow(m_timeNoContact, 2)) / 8 * 100;

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
                    auto *itemFall = new QStandardItem(QString::number(ui->edFallHeight->value()));
                    itemFall->setEditable(false);
                    itemFall->setData(ui->edFallHeight->value(), ValueRole);
                    auto *itemH = new QStandardItem(QString::number(height));
                    itemH->setEditable(false);
                    itemH->setData(height, ValueRole);
                    auto *itemPower = new QStandardItem(QString::number(power));
                    itemPower->setEditable(false);
                    auto *itemStiff = new QStandardItem(QString::number(stiffness));
                    itemStiff->setEditable(false);
                    auto *itemIS = new QStandardItem(QString::number(initialSpeed));
                    itemIS->setEditable(false);
                    auto *itemRSI = new QStandardItem(QString::number(rsi));
                    itemRSI->setEditable(false);

                    m_mdlTable.appendRow(QList<QStandardItem*>() << itemN << itemTC << itemTV << itemM << itemFall
                                         << itemH << itemPower << itemStiff << itemIS << itemRSI);

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
                                         << tr("Высота\nспрыгивания, см")
                                         << tr("Высота\nпрыжка, см")
                                         << tr("Мощность")
                                         << tr("Жесткость")
                                         << tr("Начальная\nскорость, см/сек")
                                         << tr("Индекс\nпрыгучести"));
    ui->tvJumps->header()->resizeSections(QHeaderView::ResizeToContents);
    ui->tvJumps->header()->resizeSection(0, 100);
}

void DropTestExecute::finishTest()
{
    m_isRecording = false;

    MetodicDefines::MetodicInfo mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedMetodic();

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