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
    Q_UNUSED(params);
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

        if (m_jumpControl)
        {
            m_plt1Pressed = m_jumpControl->platformState(0);
            m_plt2Pressed = m_jumpControl->platformState(1);
            mainMsgData(true);
        }
    }
    else
    {
        QMessageBox::warning(this, tr("Предупреждение"), tr("Отсутствует необходимое подключение для работы теста"));
        static_cast<ExecuteWidget*>(parent())->showDB();
    }
}

int n = 0;

void JumpTestExecute::getData(DeviceProtocols::DeviceData *data)
{
    if (data->uid() == DeviceProtocols::uid_JumpPlateBlockData)
    {
        DeviceProtocols::JumpPlateBlockData *jpData = static_cast<DeviceProtocols::JumpPlateBlockData*>(data);

        jpData->counter1();
        ui->lblBlockCounter->setText(QString("Пакеты : %1.  Платформа 1: Загрузка: %2, Счетчик: %3, Константа: %4.   Платформа 2: Загрузка: %5, Счетчик: %6, Константа: %7").
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

        mainMsgData(false);

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

void JumpTestExecute::mainMsgData(const bool isStart)
{
    int pltCnt = 1;
    if (m_jumpControl)
        pltCnt = m_jumpControl->platformsCount();

    if (pltCnt == 2)
    {
        ui->lblStateOnPlatform->setVisible(!(m_plt1Pressed && m_plt2Pressed));
        ui->lblRunJump->setVisible(m_plt1Pressed && m_plt2Pressed);

        if (m_plt1Pressed && m_plt2Pressed &&
            m_plt1Height > 0 && m_plt1Height < 3 &&
            m_plt2Height > 0 && m_plt2Height < 3 &&
            ! isStart)
        {
            ui->lblJumpHeight->setVisible(true);
            double mid = (m_plt1Height + m_plt2Height) / 2;
            ui->lblJumpHeight->setText(QString(tr("Высота прыжка, м") + " : %1").arg(mid));
        }
    }
    else
    if (pltCnt == 1)
    {
        ui->lblStateOnPlatform->setVisible(!m_plt1Pressed);
        ui->lblRunJump->setVisible(m_plt1Pressed);

        if (m_plt1Pressed &&
            m_plt1Height > 0 && m_plt1Height < 3 &&
            ! isStart)
        {
            ui->lblJumpHeight->setVisible(true);
            double mid = m_plt1Height / 2;
            ui->lblJumpHeight->setText(QString(tr("Высота прыжка, м") + " : %1").arg(mid));
        }
    }

}
