#include "jumptestexecute.h"
#include "ui_jumptestexecute.h"

#include "aanalyserapplication.h"
#include "testresultdata.h"
#include "executewidget.h"
#include "driver.h"
#include "jumptestdefines.h"
#include "deviceprotocols.h"
#include "jumpplatedata.h"
#include "channelsdefines.h"
#include "settingsprovider.h"

#include <QTimer>
#include <QMessageBox>
#include <QDebug>

JumpTestExecute::JumpTestExecute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JumpTestExecute)
{
    ui->setupUi(this);
    ui->lblCommunicationError->setVisible(false);
    ui->lblJumpHeight->setVisible(false);
    ui->frTools->setVisible(false);
    ui->btnSave->setEnabled(false);
    QTimer::singleShot(0, this, &JumpTestExecute::start);
}

JumpTestExecute::~JumpTestExecute()
{
    if (m_driver)
    {
        m_driver->stop();
        m_driver->deleteLater();
    }
    delete ui;
}

void JumpTestExecute::setParams(const QJsonObject &params)
{
    m_methodic = static_cast<JumpTestDefines::Methodic>(params["methodic"].toInt());
    if (m_methodic == JumpTestDefines::MetJumpHeight)
        ui->lblStateOnPlatform->setText(tr("Станьте на прыжковую платформу"));
    else
    if (m_methodic == JumpTestDefines::MetHopping)
        ui->lblStateOnPlatform->setText(tr("Сойдите с прыжковой платформы"));
}

void JumpTestExecute::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_timerScenFailure)
    {
        if (m_hoppingStage == m_scenFailureStage)
        {
            ui->lblJumpHeight->setText(tr("Срыв сценария. Встаньте на платформу и сойдите с нее"));
        }
        killTimer(m_timerScenFailure);
    }

    QWidget::timerEvent(event);
}

void JumpTestExecute::start()
{
    m_g = SettingsProvider::valueFromRegAppCopy("UserLocalize", "g", static_cast<QVariant>(9.8)).toDouble();

    m_driver = static_cast<AAnalyserApplication*>(QApplication::instance())->
            getDriverByProtocols(QStringList() << DeviceProtocols::uid_JumpPlateProtocol);
    if (m_driver)
    {
        m_jumpControl = dynamic_cast<DeviceProtocols::JumpPlateControl*>(m_driver);

        connect(m_driver, &Driver::sendData, this, &JumpTestExecute::getData);
        connect(m_driver, &Driver::communicationError, this, &JumpTestExecute::on_communicationError);

        m_driver->start();

        QTimer::singleShot(300, [=]()
        {
            if (m_jumpControl)
            {
                m_isBlocked = false;
                m_plt1Pressed = m_jumpControl->platformState(0);
                m_plt2Pressed = m_jumpControl->platformState(1);
                methodicWorking(true);
            }
        });
    }
    else
    {
        QMessageBox::warning(this, tr("Предупреждение"), tr("Отсутствует необходимое подключение для работы теста"));
        static_cast<ExecuteWidget*>(parent())->showDB();
    }
}

void JumpTestExecute::getData(DeviceProtocols::DeviceData *data)
{
    if (data->uid() == DeviceProtocols::uid_JumpPlateBlockData)
    {
        DeviceProtocols::JumpPlateBlockData *jpData = static_cast<DeviceProtocols::JumpPlateBlockData*>(data);

        jpData->counter1();
        ui->lblBlockCounter->setText(QString("Пакеты : %1.\nПлатформа 1: Загрузка: %2, Счетчик: %3, Константа: %4.\nПлатформа 2: Загрузка: %5, Счетчик: %6, Константа: %7").
                                     arg(jpData->blockCnt()).
                                     arg(jpData->busy1()).arg((unsigned int)jpData->counter1()).arg(jpData->con1()).
                                     arg(jpData->busy2()).arg((unsigned int)jpData->counter2()).arg(jpData->con2()));
    }
    else
    if (data->uid() == DeviceProtocols::uid_JumpPlateDvcData)
    {
        ui->lblCommunicationError->setVisible(false);

        DeviceProtocols::JumpPlateDvcData *jpData = static_cast<DeviceProtocols::JumpPlateDvcData*>(data);

        if (jpData->plate() == 1)
        {
            ++m_plt1Count;
            ui->lblPlate1Flag->setText(JumpTestDefines::BusyValue.value(jpData->busy()));
            ui->lblPlate1Time->setText(QString(tr("Время, мс") + " : %1").arg(jpData->time()));
            double h = (m_g * pow(jpData->time() / 1000, 2)) / 8;
            ui->lblPlate1Height->setText(QString(tr("Высота прыжка, м") + " : %1").arg(h));
            ui->lblPlate1Count->setText(QString(tr("Кол-во") + " : %1").arg(m_plt1Count));

            m_plt1Pressed = jpData->busy();
            m_plt1Time = jpData->time() / 1000;
            if (m_plt1Pressed)
                m_plt1Height = h;
        }
        else
        if (jpData->plate() == 2)
        {
            ++m_plt2Count;
            ui->lblPlate2Flag->setText(JumpTestDefines::BusyValue.value(jpData->busy()));
            ui->lblPlate2Time->setText(QString(tr("Время, мс") + " : %1").arg(jpData->time()));
            double h = (m_g * pow(jpData->time() / 1000, 2)) / 8;
            ui->lblPlate2Height->setText(QString(tr("Высота прыжка, м") + " : %1").arg(h));
            ui->lblPlate2Count->setText(QString(tr("Кол-во") + " : %1").arg(m_plt2Count));

            m_plt2Pressed = jpData->busy();
            m_plt2Time = jpData->time() / 1000;
            if (m_plt2Pressed)
                m_plt2Height = h;
        }

        methodicWorking(false);

//        ui->lblX->setText(QString("X = %1").arg(jpData->x(), 0, 'f', 2));
//        ui->lblY->setText(QString("Y = %1").arg(jpData->y(), 0, 'f', 2));
//        ui->lblZ->setText(QString("Z = %1").arg(jpData->z(), 0, 'f', 2));
//        ui->wgtSKG->setMarker(jpData->x(), jpData->y());

//        if (m_isRecording)
//        {
//            SignalsDefines::StabRec rec(std::make_tuple(jpData->x(), jpData->y()));
//            m_stb->addValue(rec);
//            m_z->addValue(jpData->x() + jpData->y());

//            ++m_recCount;
//            if (probeParams().autoEnd)
//            {
//                ui->lblRecLen->setText(BaseUtils::getTimeBySecCount(m_recCount / m_frequency) + " / " +
//                                       BaseUtils::getTimeBySecCount(probeParams().time));
//                double rc = m_recCount;
//                double mrc = probeParams().time * m_frequency;
//                ui->pbRec->setValue(rc / mrc * 100);
//            }
//            else
//                ui->lblRecLen->setText(BaseUtils::getTimeBySecCount(m_recCount / m_frequency));

//            if (probeParams().autoEnd && (m_recCount >= probeParams().time * m_frequency))
//            {
//                //! Следующая проба
//                nextProbe();
//                //! Пробы кончились - завершение
//                if (m_probe >= m_params.size())
//                    finishTest();
//            }
//        }
    }
}

void JumpTestExecute::on_communicationError(const QString &drvName, const QString &port, const int errorCode)
{
    Q_UNUSED(errorCode);
    ui->lblCommunicationError->setText(QString(tr("Ошибка связи с устройством") + ": %1 (" + tr("порт") + ": %2)").
                                       arg(drvName).arg(port));
    ui->lblCommunicationError->setVisible(true);
}

void JumpTestExecute::showTools(bool isShow)
{
    ui->frTools->setVisible(isShow);
}

void JumpTestExecute::saveResult()
{
    auto kard = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedPatient();
    MetodicDefines::MetodicInfo mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedMetodic();

    TestResultData m_trd;  ///< Объект, записывающий данные в базу
    m_trd.newTest(kard.uid, mi.uid);
    if (m_methodic == JumpTestDefines::MetJumpHeight)
    {
        m_trd.newProbe(tr("Анализ высоты прыжка"));
        auto *data = new JumpHeightSingleData(ChannelsDefines::chanJumpSingleHeight);
        data->setHeight((m_plt1HeightRes + m_plt2HeightRes) / 2);
        m_trd.addChannel(data);
    }
    else
    if (m_methodic == JumpTestDefines::MetHopping)
    {
        m_trd.newProbe(tr("Анализ соскакивания"));
        auto *data = new HoppingData(ChannelsDefines::chanHopping);
        data->setHeight((m_plt1HeightRes + m_plt2HeightRes) / 2);
        data->setTime((m_plt1HoppingTimeOn + m_plt2HoppingTimeOn) / 2);
        m_trd.addChannel(data);
    }
    m_trd.saveTest();
    static_cast<ExecuteWidget*>(parent())->showDB();
}

void JumpTestExecute::cancelTest()
{
    static_cast<ExecuteWidget*>(parent())->showDB();
}

void JumpTestExecute::methodicWorking(const bool isStart)
{
    if (m_isBlocked)
        return;
    int pltCnt = 1;
    if (m_jumpControl)
        pltCnt = m_jumpControl->platformsCount();

    if (m_methodic == JumpTestDefines::MetJumpHeight)
        methodicWorkingJumpHeight(isStart, pltCnt);
    else
    if (m_methodic == JumpTestDefines::MetHopping)
        methodicWorkingHopping(isStart, pltCnt);
}

void JumpTestExecute::methodicWorkingJumpHeight(const bool isStart, const int platformsCount)
{
    if (platformsCount == 2)
    {
        ui->lblStateOnPlatform->setVisible(!(m_plt1Pressed && m_plt2Pressed));
        ui->lblRunJump->setVisible(m_plt1Pressed && m_plt2Pressed);

        if (m_plt1Pressed && m_plt2Pressed &&
            m_plt1Height > 0 && m_plt1Height < 3 &&
            m_plt2Height > 0 && m_plt2Height < 3 &&
            ! isStart)
        {
            m_plt1TimeRes = m_plt1Time;
            m_plt1HeightRes = m_plt1Height;
            m_plt2TimeRes = m_plt2Time;
            m_plt2HeightRes = m_plt2Height;
            ui->lblJumpHeight->setVisible(true);
            ui->btnSave->setEnabled(true);
            double mid = (m_plt1HeightRes + m_plt2HeightRes) / 2;
            ui->lblJumpHeight->setText(QString(tr("Высота прыжка, м") + " : %1").arg(mid));
        }
    }
    else
    if (platformsCount == 1)
    {
        ui->lblStateOnPlatform->setVisible(!m_plt1Pressed);
        ui->lblRunJump->setVisible(m_plt1Pressed);

        if (m_plt1Pressed &&
            m_plt1Height > 0 && m_plt1Height < 3 &&
            ! isStart)
        {
            m_plt1TimeRes = m_plt1Time;
            m_plt1HeightRes = m_plt1Height;
            m_plt2HeightRes = m_plt1Height;
            ui->lblJumpHeight->setVisible(true);
            ui->btnSave->setEnabled(true);
            double mid = m_plt1HeightRes;
            ui->lblJumpHeight->setText(QString(tr("Высота прыжка, м") + " : %1").arg(mid));
        }
    }

}

void JumpTestExecute::methodicWorkingHopping(const bool isStart, const int platformsCount)
{
    Q_UNUSED(isStart);
    bool isOnePressed = m_plt1Pressed || m_plt2Pressed;
    bool isBothPressed = m_plt1Pressed && m_plt2Pressed;
    bool isNotPressed = !m_plt1Pressed && !m_plt2Pressed;
    if (platformsCount == 1)
    {
        isOnePressed = m_plt1Pressed;
        isBothPressed = m_plt1Pressed;
        isNotPressed = !m_plt1Pressed;
    }

    if (m_hoppingStage == -1)
    {
        if (isOnePressed)
        {
            ui->lblStateOnPlatform->setVisible(true);
            ui->lblRunJump->setVisible(false);
        }
        else
        {
            ui->lblStateOnPlatform->setVisible(false);
            ++m_hoppingStage;
            ui->lblRunJump->setText(tr("Выполните прыжок на платформу"));
            ui->lblRunJump->setVisible(true);
        }
    }
    else
    if (m_hoppingStage == 0)
    {
//        ui->btnSave->setEnabled(false); Если включить, то будет кнопка [Сохранить] гаснуть при сходе с платформы
        if (isBothPressed)
        {
            ++m_hoppingStage;
            ui->lblRunJump->setText(tr("Выполните прыжок"));
            m_scenFailureStage = m_hoppingStage;
            m_timerScenFailure = startTimer(3000);
        }
    }
    else
    if (m_hoppingStage == 1)
    {
        if (isNotPressed)
        {
            ++m_hoppingStage;
            ui->lblRunJump->setVisible(false);
            m_plt1HoppingTimeOn = m_plt1Time;
            if (platformsCount == 2)
                m_plt2HoppingTimeOn = m_plt2Time;
            else
            if (platformsCount == 1)
                m_plt2HoppingTimeOn = m_plt1Time;
            m_scenFailureStage = m_hoppingStage;
            m_timerScenFailure = startTimer(3000);
        }
    }
    else
    if (m_hoppingStage == 2)
    {
        if (isBothPressed)
        {
            m_plt1TimeRes = m_plt1Time;
            m_plt1HeightRes = m_plt1Height;
            if (platformsCount == 2)
            {
                m_plt2TimeRes = m_plt2Time;
                m_plt2HeightRes = m_plt2Height;
            }
            else
            if (platformsCount == 1)
            {
                m_plt2TimeRes = m_plt1Time;
                m_plt2HeightRes = m_plt1Height;
            }
            m_hoppingStage = -1;

            ui->lblRunJump->setVisible(false);
            ui->lblStateOnPlatform->setVisible(true);
            ui->lblJumpHeight->setVisible(true);
            ui->btnSave->setEnabled(true);

            double time = (m_plt1HoppingTimeOn + m_plt2HoppingTimeOn) / 2;
            double height = (m_plt1HeightRes + m_plt2HeightRes) / 2;
            ui->lblJumpHeight->setText(QString(tr("Время на платформе, сек") + " : %1;    " +
                                               tr("Высота прыжка, м") + " : %2").arg(time).arg(height));
        }
    }
}
