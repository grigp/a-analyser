#include "stabtestexecute.h"
#include "ui_stabtestexecute.h"

#include "executewidget.h"

#include "stabilogram.h"
#include "ballistogram.h"
#include "channelsdefines.h"
#include "channelsutils.h"
#include "testresultdata.h"
#include "baseutils.h"
#include "aanalyserapplication.h"
#include "deviceprotocols.h"
#include "driver.h"
#include "settingsprovider.h"

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

    ui->wgtAdvChannels->setVisible(false);
    ui->btnCalibrate->setVisible(false);
}

StabTestExecute::~StabTestExecute()
{
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

void StabTestExecute::closeEvent(QCloseEvent *event)
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

void StabTestExecute::start()
{
//    m_driver = static_cast<AAnalyserApplication*>(QApplication::instance())->
//            getDriverByProtocols(QStringList() << DeviceProtocols::uid_StabProtocol);
    //! Запрашиваем не протокол, а формат канала, тогда будем работать с любыми данными,
    //! представляющими собой точку в декартовых координатах, а не только стабилограмму
    m_driver = static_cast<AAnalyserApplication*>(QApplication::instance())->
            getDriverByFormats(QStringList() << ChannelsDefines::cfDecartCoordinates);
    if (m_driver)
    {
        m_stabControl = dynamic_cast<DeviceProtocols::StabControl*>(m_driver);
        m_freqStab = m_driver->frequency(ChannelsDefines::chanStab);
        m_freqZ = m_driver->frequency(ChannelsDefines::chanZ);

        connect(m_driver, &Driver::sendData, this, &StabTestExecute::getData);
        connect(m_driver, &Driver::communicationError, this, &StabTestExecute::on_communicationError);

        setChannels();

        m_kard = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedPatient();
        MetodicDefines::MetodicInfo mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedMetodic();
        ui->lblProbeTitle->setText(probeParams().name + " - " + m_kard.fio);
        m_trd->newTest(m_kard.uid, mi.uid);

        showPatientWindow(m_params.at(m_probe).stimulCode);
        ui->cbScale->setCurrentIndex(m_params.at(m_probe).scale);

        ui->wgtAdvChannels->assignDriver(m_driver, m_trd);
        //! Стабилограмма будет записана всегда
        ui->wgtAdvChannels->setAllwaysRecordingChannel(ui->cbSelectChannel->currentData(ChannelsUtils::ChannelUidRole).toString());
        auto val = SettingsProvider::valueFromRegAppCopy("AdvancedChannelsWidget", "SplitterProbePosition").toByteArray();
        ui->splitter->restoreState(val);

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
//    if (data->uid() == DeviceProtocols::uid_StabDvcData)
    //! Выбранный в переключателе канал, а не просто данные.
    //! Если драйвер будет передавать несколько стабилограмм, то отображать здесь мы будем только одну
    if (ui->cbSelectChannel->currentData(ChannelsUtils::ChannelUidRole).toString() == data->channelId())
    {
        ui->lblCommunicationError->setVisible(false);

        DeviceProtocols::StabDvcData *stabData = static_cast<DeviceProtocols::StabDvcData*>(data);
        ui->lblX->setText(QString("X = %1").arg(stabData->x(), 0, 'f', 2));
        ui->lblY->setText(QString("Y = %1").arg(stabData->y(), 0, 'f', 2));
        ui->lblZ->setText(QString("Z = %1").arg(stabData->z(), 0, 'f', 2));
        ui->wgtSKG->setMarker(stabData->x(), stabData->y());

        ui->wgtAdvChannels->getData(data);

        if (m_patientWin)
            m_patientWin->setMarker(stabData->x(), stabData->y());

        if (m_isRecording)
        {
            //! Запись, если не задержка привыкания
            if (m_recCount >= probeParams().latentTime * m_freqStab)
            {
                ui->wgtAdvChannels->record(data);
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
        m_stabControl->zeroing(ChannelsDefines::chanStab);
}

void StabTestExecute::calibrate()
{
    if (m_stabControl)
        m_stabControl->calibrate(ChannelsDefines::chanStab);
}

void StabTestExecute::recording()
{
    m_isRecording = ! m_isRecording;

    ui->pbRec->setValue(0);
    ui->lblRecLen->setText("00:00");

    ui->btnZeroing->setEnabled(!m_isRecording);
    ui->btnCalibrate->setEnabled(!m_isRecording);
    ui->frScale->setEnabled(!m_isRecording);
    ui->wgtAdvChannels->enabledControls(!m_isRecording);

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

void StabTestExecute::on_advChannelsClicked(bool checked)
{
    ui->wgtAdvChannels->setVisible(checked);
}

void StabTestExecute::splitterMoved(int pos, int index)
{
    Q_UNUSED(pos);
    Q_UNUSED(index);
    SettingsProvider::setValueToRegAppCopy("AdvancedChannelsWidget", "SplitterProbePosition", ui->splitter->saveState());
}

StabTestParams::ProbeParams StabTestExecute::probeParams()
{
    if (m_probe >= 0 && m_probe < m_params.size())
        return m_params.at(m_probe);
    else
        return StabTestParams::ProbeParams();
}

void StabTestExecute::setChannels()
{
    auto listChanUid = m_driver->getChannelsByFormat(ChannelsDefines::cfDecartCoordinates);
    foreach (auto channelUid, listChanUid)
    {
        auto subChanCnt = m_driver->getSubChannelsCount(channelUid);

        for (int i = 0; i < subChanCnt; ++i)
        {
            auto name = ChannelsUtils::instance().channelName(channelUid);
            ui->cbSelectChannel->addItem(name);
            ui->cbSelectChannel->setItemData(ui->cbSelectChannel->count() - 1, channelUid, ChannelsUtils::ChannelUidRole);
            ui->cbSelectChannel->setItemData(ui->cbSelectChannel->count() - 1, i, ChannelsUtils::SubChanNumRole);
        }
    }
    ui->frSelectChannel->setVisible(listChanUid.size() > 1);
}

void StabTestExecute::initRecSignals()
{
    ///< Запись данных в пробе
    if (m_probe < m_params.size())
    {
        if (m_probe == m_trd->probesCount())   //! Новая проба - создать пробу и сигналы
        {
            m_trd->newProbe(probeParams().name);
            ui->wgtAdvChannels->newProbe();
        }
        else
        {                               //! Проба была прервана - очистить сигналы
            ui->wgtAdvChannels->abortProbe();
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
        ui->wgtAdvChannels->enabledControls(!m_isRecording);
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
        auto rect = QApplication::desktop()->screenGeometry(0);
        if (QApplication::desktop()->screenCount() > 1)
            rect = QApplication::desktop()->screenGeometry(1);

        m_patientWin->resize(rect.size());
        m_patientWin->move(rect.x(), rect.y());

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
