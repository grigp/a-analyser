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
    }
    else
    {
        QMessageBox::warning(this, tr("Предупреждение"), tr("Отсутствует необходимое подключение для работы теста"));
        static_cast<ExecuteWidget*>(parent())->showDB();
    }
}

void JumpTestExecute::getData(DeviceProtocols::DeviceData *data)
{
    if (data->uid() == DeviceProtocols::uid_JumpPlateDvcData)
    {
        ui->lblCommunicationError->setVisible(false);

        DeviceProtocols::JumpPlateDvcData *jpData = static_cast<DeviceProtocols::JumpPlateDvcData*>(data);

        if (jpData->plate() == 1)
        {
            ui->lblPlate1Flag->setText(JumpTestDefines::BusyValue.value(jpData->busy()));
            ui->lblPlate1Time->setText(QString(tr("Время, сек") + " : %1").arg(jpData->time()));
            ui->lblPlate1Height->setText(QString(tr("Высота прыжка, мм") + " : %1").arg(jpData->time()));
        }
        else
        if (jpData->plate() == 2)
        {
            ui->lblPlate2Flag->setText(JumpTestDefines::BusyValue.value(jpData->busy()));
            ui->lblPlate2Time->setText(QString(tr("Время, сек") + " : %1").arg(jpData->time()));
            ui->lblPlate2Height->setText(QString(tr("Высота прыжка, мм") + " : %1").arg(jpData->time()));
        }

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
