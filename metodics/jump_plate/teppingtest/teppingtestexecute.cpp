#include "teppingtestexecute.h"
#include "ui_teppingtestexecute.h"

#include "aanalyserapplication.h"
#include "driver.h"
#include "executewidget.h"

#include <QTimer>
#include <QMessageBox>

TeppingTestExecute::TeppingTestExecute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TeppingTestExecute)
{
    ui->setupUi(this);
    ui->lblCommunicationError->setVisible(false);
    QTimer::singleShot(0, this, &TeppingTestExecute::start);
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
}

void TeppingTestExecute::start()
{
    m_driver = static_cast<AAnalyserApplication*>(QApplication::instance())->
            getDriverByProtocols(QStringList() << DeviceProtocols::uid_JumpPlateProtocol);
    if (m_driver)
    {
        m_jumpControl = dynamic_cast<DeviceProtocols::JumpPlateControl*>(m_driver);

        connect(m_driver, &Driver::sendData, this, &TeppingTestExecute::getData);
        connect(m_driver, &Driver::communicationError, this, &TeppingTestExecute::on_communicationError);

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

//        ui->tvJumps->setModel(&m_mdlTable);
//        setModelGeometry();
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
        ui->btnSave->setIcon(QIcon(":/images/SaveNO.png"));
        ui->btnSave->setText(tr("Прервать шаги"));
        m_timerEndOfTest = startTimer(20);
    }
    else
    {
        ui->btnSave->setIcon(QIcon(":/images/Save.png"));
        ui->btnSave->setText(tr("Начать шаги"));
        killTimer(m_timerEndOfTest);
    }

    ui->pbTimeTest->setValue(0);
    m_stepsLeftCount = 0;
    m_stepsRightCount = 0;
    m_timeCount = 0;
    m_time = 0;
//    m_mdlTable.clear();
    setModelGeometry();
}

void TeppingTestExecute::iterate(const bool isStart)
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
    }

}

void TeppingTestExecute::setModelGeometry()
{

}

void TeppingTestExecute::finishTest()
{

}
