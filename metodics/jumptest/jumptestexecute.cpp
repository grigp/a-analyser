#include "jumptestexecute.h"
#include "ui_jumptestexecute.h"

#include "aanalyserapplication.h"
#include "testresultdata.h"
#include "executewidget.h"
#include "driver.h"
#include "jumptestdefines.h"
#include "deviceprotocols.h"

#include <QTimer>
#include <QMessageBox>
#include <QDebug>

JumpTestExecute::JumpTestExecute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JumpTestExecute)
  , m_trd(new TestResultData())
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

void JumpTestExecute::start()
{
    m_driver = static_cast<AAnalyserApplication*>(QApplication::instance())->
            getDriver(QStringList() << DeviceProtocols::uid_JumpPlateProtocol);
    if (m_driver)
    {
        m_jumpControl = dynamic_cast<DeviceProtocols::JumpPlateControl*>(m_driver);

        connect(m_driver, &Driver::sendData, this, &JumpTestExecute::getData);
        connect(m_driver, &Driver::communicationError, this, &JumpTestExecute::on_communicationError);

        auto kard = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedPatient();
        MetodicDefines::MetodicInfo mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedMetodic();
        m_trd->newTest(kard.uid, mi.uid);

        m_driver->start();

        QTimer::singleShot(300, [=]()
        {
            if (m_jumpControl)
            {
                m_isBlocked = false;
                m_plt1Pressed = m_jumpControl->platformState(0);
                m_plt2Pressed = m_jumpControl->platformState(1);
                qDebug() << "1" << m_plt1Pressed << m_plt2Pressed;
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
//        if (jpData->blockCnt() == 2)
//        {
//            m_plt1Pressed = jpData->busy1();
//            m_plt2Pressed = jpData->busy2();
//            methodicWorking(true);
//        }
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
            double h = (9.8 * pow(jpData->time() / 1000, 2)) / 8;
            ui->lblPlate1Height->setText(QString(tr("Высота прыжка, м") + " : %1").arg(h));
            ui->lblPlate1Count->setText(QString(tr("Кол-во") + " : %1").arg(m_plt1Count));

            m_plt1Pressed = jpData->busy();
            if (m_plt1Pressed)
            {
                m_plt1Time = jpData->time();
                m_plt1Height = h;
            }
        }
        else
        if (jpData->plate() == 2)
        {
            ++m_plt2Count;
            ui->lblPlate2Flag->setText(JumpTestDefines::BusyValue.value(jpData->busy()));
            ui->lblPlate2Time->setText(QString(tr("Время, мс") + " : %1").arg(jpData->time()));
            double h = (9.8 * pow(jpData->time() / 1000, 2)) / 8;
            ui->lblPlate2Height->setText(QString(tr("Высота прыжка, м") + " : %1").arg(h));
            ui->lblPlate2Count->setText(QString(tr("Кол-во") + " : %1").arg(m_plt2Count));

            m_plt2Pressed = jpData->busy();
            if (m_plt2Pressed)
            {
                m_plt2Time = jpData->time();
                m_plt2Height = h;
            }
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
            ui->lblJumpHeight->setVisible(true);
            ui->btnSave->setEnabled(true);
            double mid = (m_plt1Height + m_plt2Height) / 2;
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
            ui->lblJumpHeight->setVisible(true);
            ui->btnSave->setEnabled(true);
            double mid = m_plt1Height / 2;
            ui->lblJumpHeight->setText(QString(tr("Высота прыжка, м") + " : %1").arg(mid));
        }
    }

}

void JumpTestExecute::methodicWorkingHopping(const bool isStart, const int platformsCount)
{
    qDebug() << isStart << m_hoppingStage << m_plt1Pressed << m_plt2Pressed;
    if (m_hoppingStage == -1)
    {
        if (m_plt1Pressed || m_plt2Pressed)
            ui->lblStateOnPlatform->setVisible(true);
        else
        {
            ui->lblStateOnPlatform->setVisible(false);
            ++m_hoppingStage;
            ui->lblRunJump->setText(tr("Выполните прыжок на платформу"));
        }
    }
    else
    if (m_hoppingStage == 0)
    {
        if (m_plt1Pressed && m_plt2Pressed)
        {
            ++m_hoppingStage;
            ui->lblRunJump->setText(tr("Выполните прыжок"));
        }
    }
    else
    if (m_hoppingStage == 1)
    {
        if (!m_plt1Pressed && !m_plt2Pressed)
        {
            ++m_hoppingStage;
            ui->lblRunJump->setVisible(false);
        }
    }
    else
    if (m_hoppingStage == 2)
    {
        if (m_plt1Pressed && m_plt2Pressed)
        {
            m_hoppingStage = -1;
            ui->lblRunJump->setVisible(false);
            ui->lblStateOnPlatform->setVisible(true);
            ui->lblJumpHeight->setVisible(true);
        }
    }
    ui->label->setText(QString::number(m_hoppingStage));
}
