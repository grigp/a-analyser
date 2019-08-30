#include "stabtestexecute.h"
#include "ui_stabtestexecute.h"

#include "executewidget.h"

#include "stabilogram.h"
#include "ballistogram.h"
#include "channelsdefines.h"
#include "testresultdata.h"
#include "baseutils.h"
#include "aanalyserapplication.h"
#include "deviceprotocols.h"
#include "driver.h"

#include "coloredcirclewindow.h"
#include "soundpickwindow.h"
#include "targetwindow.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QComboBox>
#include <QDebug>

StabTestExecute::StabTestExecute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StabTestExecute)
  , m_trd(new TestResultData())
{
    ui->setupUi(this);

    ui->lblCommunicationError->setVisible(false);
    QTimer::singleShot(0, this, &StabTestExecute::start);

    ui->cbScale->addItem("1");
    ui->cbScale->addItem("2");
    ui->cbScale->addItem("4");
    ui->cbScale->addItem("8");
    ui->cbScale->addItem("16");
    ui->cbScale->addItem("32");
    ui->cbScale->addItem("64");
    ui->cbScale->addItem("128");
}

StabTestExecute::~StabTestExecute()
{
    if (m_driver)
    {
        m_driver->stop();
        m_driver->deleteLater();
    }
    delete ui;
}

void StabTestExecute::setParams(const QJsonObject &params)
{
    m_params.clear();
    auto prbsArr = params["probes"].toArray();
    for (int i = 0; i < prbsArr.size(); ++i)
    {
        auto obj = prbsArr[i].toObject();
        StabTestParams::ProbeParams pp;

        pp.name = obj["name"].toString();
        pp.autoEnd = obj["autoend"].toInt() == 1;
        pp.time = obj["time"].toInt();
        pp.latentTime = obj["latent_time"].toInt();
        pp.stimulCode = obj["stimul"].toInt();
        pp.zeroingEnabled = obj["zeroing"].toInt() == 1;
        pp.scale = obj["scale"].toInt();

        m_params << pp;
    }

    ui->lblProbeTitle->setText(probeParams().name + " - " + m_kard.fio);
}

void StabTestExecute::start()
{
    m_driver = static_cast<AAnalyserApplication*>(QApplication::instance())->
            getDriver(QStringList() << DeviceProtocols::uid_StabProtocol);
    if (m_driver)
    {
        m_stabControl = dynamic_cast<DeviceProtocols::StabControl*>(m_driver);
        m_freqStab = m_driver->frequency(ChannelsDefines::chanStab);
        m_freqZ = m_driver->frequency(ChannelsDefines::chanZ);

        connect(m_driver, &Driver::sendData, this, &StabTestExecute::getData);
        connect(m_driver, &Driver::communicationError, this, &StabTestExecute::on_communicationError);

        m_kard = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedPatient();
        MetodicDefines::MetodicInfo mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedMetodic();
        ui->lblProbeTitle->setText(probeParams().name + " - " + m_kard.fio);
        m_trd->newTest(m_kard.uid, mi.uid);

        showPatientWindow(m_params.at(m_probe).stimulCode);
        ui->cbScale->setCurrentIndex(m_params.at(m_probe).scale);

        m_driver->start();
    }
    else
    {
        QMessageBox::warning(this, tr("Предупреждение"), tr("Отсутствует необходимое подключение для работы теста"));
        static_cast<ExecuteWidget*>(parent())->showDB();
    }
}

void StabTestExecute::scaleChange(int scaleId)
{
    int v = 1;
    for (int i = 0; i < scaleId; ++i)
        v = v * 2;
    ui->wgtSKG->setDiap(128 / v);
    if (m_patientWin)
        m_patientWin->setDiap(128 / v);
}

void StabTestExecute::getData(DeviceProtocols::DeviceData *data)
{
    if (data->uid() == DeviceProtocols::uid_StabDvcData)
    {
        ui->lblCommunicationError->setVisible(false);

        DeviceProtocols::StabDvcData *stabData = static_cast<DeviceProtocols::StabDvcData*>(data);
//        qDebug() << stabData->sender()->driverUid() << stabData->sender()->driverName() << stabData->x() << stabData->y();
        ui->lblX->setText(QString("X = %1").arg(stabData->x(), 0, 'f', 2));
        ui->lblY->setText(QString("Y = %1").arg(stabData->y(), 0, 'f', 2));
        ui->lblZ->setText(QString("Z = %1").arg(stabData->z(), 0, 'f', 2));
        ui->wgtSKG->setMarker(stabData->x(), stabData->y());

        if (m_patientWin)
            m_patientWin->setMarker(stabData->x(), stabData->y());

        if (m_isRecording)
        {
            //! Запись, если не задержка привыкания
            if (m_recCount >= probeParams().latentTime * m_freqStab)
            {
                SignalsDefines::StabRec rec(std::make_tuple(stabData->x(), stabData->y()));
                m_stb->addValue(rec);
                m_z->addValue(stabData->z());
            }

            ++m_recCount;
            //! Вывод времени теста и прогресса
            if (probeParams().autoEnd)
            {
                double rc = m_recCount - probeParams().latentTime * m_freqStab;
                if (rc < 0)
                    rc = 0;
                if (m_recCount < probeParams().latentTime * m_freqStab)
                    ui->lblRecLen->setText(tr("Задержка привыкания"));
                else
                    ui->lblRecLen->setText(BaseUtils::getTimeBySecCount(rc / m_freqStab) + " / " +
                                           BaseUtils::getTimeBySecCount(probeParams().time));
                double mrc = probeParams().time * m_freqStab;
                ui->pbRec->setValue(rc / mrc * 100);
            }
            else
                ui->lblRecLen->setText(BaseUtils::getTimeBySecCount(m_recCount / m_freqStab));

            //! Смена пробы и окончание
            if (probeParams().autoEnd && (m_recCount >= (probeParams().time + probeParams().latentTime) * m_freqStab))
            {
                //! Следующая проба
                nextProbe();
                //! Пробы кончились - завершение
                if (m_probe >= m_params.size())
                    finishTest();
            }
        }
    }
}

void StabTestExecute::on_communicationError(const QString &drvName, const QString &port, const int errorCode)
{
    Q_UNUSED(errorCode);
    ui->lblCommunicationError->setText(QString(tr("Ошибка связи с устройством") + ": %1 (" + tr("порт") + ": %2)").
                                       arg(drvName).arg(port));
    ui->lblCommunicationError->setVisible(true);
}

void StabTestExecute::zeroing()
{
//    throw 1;
//    QLabel *lbl;   // Эмуляция сбоя
//    delete lbl;
    if (m_stabControl)
        m_stabControl->zeroing();
}

void StabTestExecute::calibrate()
{
    if (m_stabControl)
        m_stabControl->calibrate();
}

void StabTestExecute::recording()
{
    m_isRecording = ! m_isRecording;

    ui->pbRec->setValue(0);
    ui->lblRecLen->setText("00:00");

    ui->btnZeroing->setEnabled(!m_isRecording);
    ui->btnCalibrate->setEnabled(!m_isRecording);
    ui->frScale->setEnabled(!m_isRecording);

    if (m_isRecording)
    {
        if (probeParams().autoEnd)
        {
            ui->btnRecord->setIcon(QIcon(":/images/SaveNO.png"));
            ui->btnRecord->setText(tr("Прервать"));
        }
        else
        {
            ui->btnRecord->setIcon(QIcon(":/images/SaveOK.png"));
            ui->btnRecord->setText(tr("Завершить"));
        }

        initRecSignals();
        if (m_patientWin)
            m_patientWin->run();
    }
    else
    {
        if (m_patientWin)
            m_patientWin->stop();

        ui->btnRecord->setIcon(QIcon(":/images/Save.png"));
        ui->btnRecord->setText(tr("Запись"));
        if (! probeParams().autoEnd)
            finishTest();
    }
    m_recCount = 0;
}

void StabTestExecute::showTrace(bool trace)
{
    ui->wgtSKG->showTrace(trace);
}

StabTestParams::ProbeParams StabTestExecute::probeParams()
{
    if (m_probe >= 0 && m_probe < m_params.size())
        return m_params.at(m_probe);
    else
        return StabTestParams::ProbeParams();
}

void StabTestExecute::initRecSignals()
{
    ///< Запись данных в пробе
    if (m_probe < m_params.size())
    {
        if (m_probe == m_trd->probesCount())   //! Новая проба - создать пробу и сигналы
        {
            m_trd->newProbe(probeParams().name);
            m_stb = new Stabilogram(ChannelsDefines::chanStab, m_freqStab);
            m_trd->addChannel(m_stb);
            m_z = new Ballistogram(ChannelsDefines::chanZ, m_freqZ);
            m_trd->addChannel(m_z);
        }
        else
        {                               //! Проба была прервана - очистить сигналы
            m_stb->clear();
            m_z->clear();
        }
    }
}

void StabTestExecute::nextProbe()
{
    hidePatientWindow();

    ++m_probe;
    if (m_probe < m_params.size())
    {
        ui->lblProbeTitle->setText(probeParams().name + " - " + m_kard.fio);
        m_isRecording = false;
        m_recCount = 0;
        ui->lblRecLen->setText("00:00");
        ui->btnRecord->setIcon(QIcon(":/images/Save.png"));
        ui->btnRecord->setText(tr("Запись"));
        ui->pbRec->setValue(0);

        showPatientWindow(m_params.at(m_probe).stimulCode);
        ui->cbScale->setCurrentIndex(m_params.at(m_probe).scale);

        ui->btnZeroing->setVisible(m_params.at(m_probe).zeroingEnabled);
        ui->btnZeroing->setEnabled(!m_isRecording);
        ui->btnCalibrate->setEnabled(!m_isRecording);
        ui->frScale->setEnabled(!m_isRecording);
    }
}

void StabTestExecute::finishTest()
{
    hidePatientWindow();
    m_isRecording = false;
    m_trd->saveTest();
    static_cast<ExecuteWidget*>(parent())->showDB();
}

void StabTestExecute::showPatientWindow(const int winCode)
{
    switch (winCode) {
    case 1:
        m_patientWin = new ColoredCircleWindow(this);
        break;
    case 2:
        m_patientWin = new SoundPickWindow(this);
        break;
    case 3:
        m_patientWin = new TargetWindow(this);
        break;
    default:
        m_patientWin = nullptr;
        break;
    }

    if (m_patientWin)
    {
        if (QApplication::desktop()->screenCount() > 1)
            m_patientWin->setGeometry(QApplication::desktop()->screenGeometry(1));
        else
            m_patientWin->setGeometry(QApplication::desktop()->screenGeometry(0));
        m_patientWin->show();
    }
}

void StabTestExecute::hidePatientWindow()
{
    if (m_patientWin)
    {
        delete m_patientWin;
        m_patientWin = nullptr;
    }
}
