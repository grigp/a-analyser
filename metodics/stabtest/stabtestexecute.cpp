#include "stabtestexecute.h"
#include "ui_stabtestexecute.h"

#include "executewidget.h"

#include "stabilogram.h"
#include "balistogram.h"
#include "channelsdefines.h"
#include "channelsutils.h"
#include "testresultdata.h"
#include "baseutils.h"
#include "aanalyserapplication.h"
#include "deviceprotocols.h"
#include "driver.h"
#include "settingsprovider.h"
#include "bilateralresultdata.h"

#include "coloredcirclewindow.h"
#include "soundpickwindow.h"
#include "targetwindow.h"
#include "fivezoneswindow.h"
#include "lineswindow.h"
#include "driverdefines.h"
#include "aanalysersettings.h"
#include "stabtestdefines.h"
#include "amessagebox.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QDesktopWidget>
#include <QComboBox>
#include <QFileDialog>
#include <QDebug>

StabTestExecute::StabTestExecute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StabTestExecute)
  , m_trd(new TestResultData())
{
    ui->setupUi(this);

    ui->wgtSKG->addMarker();
    ui->lblCommunicationError->setVisible(false);
    ui->lblErrorInDriver->setVisible(false);
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

    ui->lblFrontComment->setStyleSheet(MetodicDefines::AutoModeMessageStyleMain);
    setFrontComment("");
}

StabTestExecute::~StabTestExecute()
{
    delete ui;
}

void StabTestExecute::setParams(const QJsonObject &params)
{
    m_params.clear();
    m_stages.clear();
    auto prbsArr = params["probes"].toArray();
    m_semanticMode = params["condition"].toInt();
    ui->gbFromFiles->setVisible(m_semanticMode == 6);
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
        if (pp.stimulCode == StabTestDefines::pwLines)
        {
            auto objSL = obj["stimul_lines"].toObject();
            pp.stimParams = objSL;
            pp.stimLines.direction = static_cast<BaseDefines::Directions>(objSL["direction"].toInt(BaseDefines::dirRight));
            pp.stimLines.width = objSL["width"].toInt(120);
            pp.stimLines.speed = objSL["speed"].toInt(200);
            pp.stimLines.dutyCycle = objSL["duty_cycle"].toInt(1);
            pp.stimLines.color = BaseUtils::strRGBAToColor(objSL["color"].toString("00000000"));
        }

        m_params << pp;
        if (pp.zeroingEnabled)
            m_stages << MetodicDefines::AutoStagesBase;
        else
            m_stages << MetodicDefines::AutoStagesWithoutZeroing;
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

void StabTestExecute::timerEvent(QTimerEvent *event)
{
    if (!ui->lblCommunicationError->isVisible() && event->timerId() == m_autoModeTimerId)
    {
        if (m_stageNum < m_stages.at(m_probe).size())
        {
            ++m_autoModeSecCounter;
            auto stage = m_stages.at(m_probe).at(m_stageNum);
            auto stageTitle = MetodicDefines::AutoModeStageTitle.value(stage);

            if (stageTitle == "")
            {
                if (m_autoModeSecCounter == m_autoTimeLatent)
                {
                    ++m_stageNum;
                    if (m_stageNum < m_stages.at(m_probe).size())
                    {
                        auto stage = m_stages.at(m_probe).at(m_stageNum);
                        auto stageTitle = MetodicDefines::AutoModeStageTitle.value(stage);
                        setFrontComment(msgWaitEvent(stageTitle, m_autoTimeRun));
                        if (m_patientWin)
                            m_patientWin->setFrontComment(msgWaitEvent(stageTitle, m_autoTimeRun));
                    }
                    m_autoModeSecCounter = 0;
                }
            }
            else
            {
                setFrontComment(msgWaitEvent(stageTitle, m_autoTimeRun - m_autoModeSecCounter));
                if (m_patientWin)
                    m_patientWin->setFrontComment(msgWaitEvent(stageTitle, m_autoTimeRun - m_autoModeSecCounter));
                if (m_autoModeSecCounter == m_autoTimeRun)
                {
                    setFrontComment("");
                    if (m_patientWin)
                        m_patientWin->setFrontComment("");
                    if (stage == MetodicDefines::amssZeroingWait)
                        zeroing();
                    else
                    if (stage == MetodicDefines::amssRecordingWait)
                        recording();
                    ++m_stageNum;
                    m_autoModeSecCounter = 0;
                }
            }
        }
    }

    QWidget::timerEvent(event);
}

void StabTestExecute::resizeEvent(QResizeEvent *event)
{
    ui->lblFrontComment->setGeometry(ui->wgtSKG->geometry());
    QWidget::resizeEvent(event);
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
        m_stabControl = static_cast<DeviceProtocols::StabControl*>(m_driver->getDeviceControl(DeviceProtocols::uid_StabControl));
        m_freqStab = m_driver->frequency(ChannelsDefines::chanStab);
        m_freqZ = m_driver->frequency(ChannelsDefines::chanZ);

        m_bilatControl = static_cast<DeviceProtocols::MultiPlatformControl*>(m_driver->getDeviceControl(DeviceProtocols::uid_MultiPlatformControl));
        if (m_bilatControl)
        {
            m_platform1 = m_bilatControl->platform(0);
            m_platform2 = m_bilatControl->platform(1);
            m_maxDiap = m_bilatControl->size();
            ui->wgtSKG->setDiap(m_maxDiap);
            ui->wgtSKG->addPlatform(m_platform1);
            ui->wgtSKG->addPlatform(m_platform2);
            ui->wgtSKG->addTarget(-100, 0, Qt::red, Qt::darkRed);
            ui->wgtSKG->addTarget(100, 0, Qt::red, Qt::darkRed);
        }

        connect(m_driver, &Driver::sendData, this, &StabTestExecute::getData);
        connect(m_driver, &Driver::communicationError, this, &StabTestExecute::on_communicationError);
        connect(m_driver, &Driver::error, this, &StabTestExecute::on_error);

        setChannels();

        m_kard = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentPatient();
        MetodicDefines::MetodicInfo mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentMetodic();
        ui->lblProbeTitle->setText(probeParams().name + " - " + m_kard.fio);
        m_trd->newTest(m_kard.uid, mi.uid);

        if (QApplication::desktop()->screenCount() > 1)
            showPatientWindow(m_params.at(m_probe).stimulCode);
        ui->cbScale->setCurrentIndex(m_params.at(m_probe).scale);
        ui->btnZeroing->setVisible(m_params.at(m_probe).zeroingEnabled);
        QTimer::singleShot(100, [&]  //! Пока процесс создания не завершен, масштаб отображается некорректно, если просто вызывать
        {
            scaleChange(ui->cbScale->currentIndex());
        });

        ui->wgtAdvChannels->assignDriver(m_driver, m_trd);
        //! Стабилограмма будет записана всегда
        ui->wgtAdvChannels->setAllwaysRecordingChannel(ui->cbSelectChannel->currentData(ChannelsUtils::ChannelUidRole).toString());
        if (m_bilatControl)
        {
            ui->wgtAdvChannels->setAllwaysRecordingChannel(ChannelsDefines::chanStabLeft);
            ui->wgtAdvChannels->setAllwaysRecordingChannel(ChannelsDefines::chanStabRight);
        }
        auto val = SettingsProvider::valueFromRegAppCopy("Geometry/AdvancedChannelsWidget", "SplitterProbePosition").toByteArray();
        ui->splitter->restoreState(val);

        m_driver->start();

        //! Запуск таймера в режиме автоматики
        auto rm = static_cast<AAnalyserApplication*>(QApplication::instance())->runningMode();
        m_autoTimeRun = SettingsProvider::valueFromRegAppCopy("", AAnalyserSettingsParams::pn_timeCounter, 5).toInt();
        m_autoTimeLatent = SettingsProvider::valueFromRegAppCopy("", AAnalyserSettingsParams::pn_timeLatent, 2).toInt();
        if (!ui->lblCommunicationError->isVisible() && rm == BaseDefines::rmAutomatic)
            m_autoModeTimerId = startTimer(1000);
    }
    else
    {
        AMessageBox::warning(this, tr("Предупреждение"), tr("Отсутствует необходимое подключение для работы теста"));
        static_cast<ExecuteWidget*>(parent())->closeExecutePage(); //showDB();
    }
}

void StabTestExecute::scaleChange(int scaleId)
{
    int v = 1;
    for (int i = 0; i < scaleId; ++i)
        v = v * 2;
    ui->wgtSKG->setDiap(m_maxDiap / v);
    if (m_patientWin)
        m_patientWin->setDiap(m_maxDiap / v);
}

void StabTestExecute::getData(DeviceProtocols::DeviceData *data)
{
//    if (data->uid() == DeviceProtocols::uid_StabDvcData)

    //! Любой канал от драйвера надо передавать окну дополнительных каналов
    ui->wgtAdvChannels->getData(data);
    if (m_isRecording)
    {
        //! Запись, если не задержка привыкания
        if (m_recCount >= probeParams().latentTime * m_freqStab)
            ui->wgtAdvChannels->record(data);
    }


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

        if (m_patientWin)
            m_patientWin->setMarker(stabData->x(), stabData->y());

        if (m_isRecording)
        {
            //! Запись, если не задержка привыкания
//            if (m_recCount >= probeParams().latentTime * m_freqStab)
//            {
//                ui->wgtAdvChannels->record(data);
//            }

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
                    ui->lblRecLen->setText(BaseUtils::getTimeBySecCount(static_cast<int>(rc / m_freqStab)) + " / " +
                                           BaseUtils::getTimeBySecCount(probeParams().time));
                double mrc = probeParams().time * m_freqStab;
                ui->pbRec->setValue(static_cast<int>(rc / mrc * 100));
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
    else
    if (data->channelId() == ChannelsDefines::chanStabLeft)
    {
        if (m_bilatControl)
        {
            DeviceProtocols::StabDvcData *stabData = static_cast<DeviceProtocols::StabDvcData*>(data);
            ui->wgtSKG->setTarget(m_platform1.center().x() + stabData->x(),
                                  m_platform1.y() - m_platform1.height() / 2 + stabData->y(), 0);
        }
    }
    else
    if (data->channelId() == ChannelsDefines::chanStabRight)
    {
        if (m_bilatControl)
        {
            DeviceProtocols::StabDvcData *stabData = static_cast<DeviceProtocols::StabDvcData*>(data);
            ui->wgtSKG->setTarget(m_platform2.center().x() + stabData->x(),
                                  m_platform2.y() - m_platform2.height() / 2 + stabData->y(), 1);
        }
    }
}

void StabTestExecute::on_communicationError(const QString &drvName, const QString &port, const int errorCode)
{
    Q_UNUSED(errorCode);
    ui->lblCommunicationError->setText(QString(tr("Ошибка связи с устройством") + ": %1 (" + tr("порт") + ": %2)").
                                       arg(drvName).arg(port));
    ui->lblCommunicationError->setVisible(true);

    //! Озвучка
    if (m_player.state() != QMediaPlayer::PlayingState)

    {
        m_player.setMedia(QUrl("qrc:/sound/com_error.wav"));
        m_player.play();
    }
}

void StabTestExecute::on_error(const int errorCode)
{
    auto eName = DriverDefines::errorName.value(static_cast<DriverDefines::ErrorCodes>(errorCode));
    ui->lblErrorInDriver->setText(QString(tr("Ошибка в драйвере") + ": %1").arg(eName));
    ui->lblErrorInDriver->setVisible(true);
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
        if (QApplication::desktop()->screenCount() == 1)
        {
            showPatientWindow(m_params.at(m_probe).stimulCode);
            ui->cbScale->setCurrentIndex(m_params.at(m_probe).scale);
            scaleChange(ui->cbScale->currentIndex());
        }
        if (m_patientWin)
            m_patientWin->run();
    }
    else
    {
        if (m_patientWin)
            m_patientWin->stop();
        if (QApplication::desktop()->screenCount() == 1)
            hidePatientWindow();

        ui->btnRecord->setIcon(QIcon(":/images/Save.png"));
        ui->btnRecord->setText(tr("Запись"));
        if (! probeParams().autoEnd)
        {
            ui->wgtAdvChannels->saveProbe();
            finishTest();
        }
    }
    m_recCount = 0;
}

void StabTestExecute::showTrace(bool trace)
{
    ui->wgtSKG->showTrace(trace);
}

void StabTestExecute::on_clearTrace()
{
    ui->wgtSKG->clearTrace();
}

void StabTestExecute::on_advChannelsClicked(bool checked)
{
    ui->wgtAdvChannels->setVisible(checked);
}

void StabTestExecute::splitterMoved(int pos, int index)
{
    Q_UNUSED(pos);
    Q_UNUSED(index);
    SettingsProvider::setValueToRegAppCopy("Geometry/AdvancedChannelsWidget", "SplitterProbePosition", ui->splitter->saveState());
}

void StabTestExecute::on_AMedStabSelect()
{
    static const QString suffix = "txt";
    static const QString emfn = QCoreApplication::tr("Текстовые файлы") + " *." + suffix + " (*." + suffix + ")";
    auto fn = QFileDialog::getOpenFileName(this, tr("Текстовые файлы"), "", emfn);
    if (fn != "")
    {
        ui->edAMedStab->setText(fn);
    }
}

void StabTestExecute::on_AMedZSelect()
{
    static const QString suffix = "txt";
    static const QString emfn = QCoreApplication::tr("Текстовые файлы") + " *." + suffix + " (*." + suffix + ")";
    auto fn = QFileDialog::getOpenFileName(this, tr("Текстовые файлы"), "", emfn);
    if (fn != "")
    {
        ui->edAMedZ->setText(fn);
    }
}

void StabTestExecute::on_MBNStabSelect()
{
    static const QString suffix = "dat";
    static const QString emfn = QCoreApplication::tr("Файлы экспорта стабилоплатформы МБН") + " *." + suffix + " (*." + suffix + ")";
    auto fn = QFileDialog::getOpenFileName(this, tr("Файлы dat"), "", emfn);
    if (fn != "")
    {
        ui->edMBNStab->setText(fn);
    }
}

void StabTestExecute::on_writeFromFiles()
{
    m_extStab1 = new Stabilogram(ChannelsDefines::chanStab1, 100);
    m_extZ1 = new Balistogram(ChannelsDefines::chanZ1, 100);
    m_extStab2 = new Stabilogram(ChannelsDefines::chanStab2, 100);
    m_extZ2 = new Balistogram(ChannelsDefines::chanZ2, 100);

    readFileAMed(ui->edAMedStab->text(), ui->edAMedZ->text(), m_extStab1, m_extZ1);
    readFileMBN(ui->edMBNStab->text(), m_extStab2, m_extZ2);

    qDebug() << m_extStab1->size() << m_extStab2->size() << m_extZ1->size() << m_extZ2->size();

    m_trd->newProbe(probeParams().name);
    m_trd->addChannel(m_extStab1);
    m_trd->addChannel(m_extZ1);
    m_trd->addChannel(m_extStab2);
    m_trd->addChannel(m_extZ2);

    finishTest();
}

void StabTestExecute::readFileAMed(const QString &fnStab, const QString &fnZ, Stabilogram *sigStab, Balistogram *sigZ)
{
    QList<SignalsDefines::StabRec> arrStab;
    arrStab.clear();

    QFile fileStab(fnStab);
    if (!fileStab.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream inStab(&fileStab);
    int n = 0;
    SignalsDefines::StabRec recO;
    while (!inStab.atEnd())
    {
        QString line = inStab.readLine();
        auto sl = line.split('\t');
        if (sl.size() == 2)
        {
            SignalsDefines::StabRec rec(std::make_tuple(sl.at(0).toDouble(), sl.at(1).toDouble()));
//            arrStab << rec;
            if (n > 0)
            {
                SignalsDefines::StabRec rec2(std::make_tuple((recO.x + rec.x) / 2,
                                                             (recO.y + rec.y) / 2));
                sigStab->addValue(rec2);
            }
            sigStab->addValue(rec);
            recO = rec;
            ++n;
        }
    }

//    for (int i = 0; i < arrStab.size() - 1; ++i)
//    {
//        SignalsDefines::StabRec rec(std::make_tuple((arrStab[i].x + arrStab[i+1].x) / 2,
//                                                    (arrStab[i].y + arrStab[i+1].y) / 2));
//        arrStab.insert(i, rec);
//    }
//    for (int i = 0; i < arrStab.size() - 1; ++i)
//        stab->addValue(arrStab[i]);


    QList<double> arrZ;
    arrZ.clear();
    QFile fileZ(fnZ);
    if (!fileZ.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream inZ(&fileZ);
    n = 0;
    double zO = 0;
    while (!inZ.atEnd())
    {
        QString line = inZ.readLine();
        double z = line.toDouble();
//        arrZ << line.toDouble();
        if (n > 0)
        {
            double z2 = (zO + z) / 2;
            sigZ->addValue(z2);
        }
        sigZ->addValue(z);
        zO = z;
        ++n;
    }

//    for (int i = 0; i < arrZ.size() - 1; ++i)
//    {
//        double z = (arrZ[i] + arrZ[i+1]) / 2;
//        arrZ.insert(i, z);
//    }
//    for (int i = 0; i < arrZ.size() - 1; ++i)
//        sigZ->addValue(arrZ[i]);
}

void StabTestExecute::readFileMBN(const QString &fn, Stabilogram *sigStab, Balistogram *sigZ)
{
    QFile file(fn);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream in(&file);
    int n = 0;
    while (!in.atEnd())
    {
        QString line = in.readLine();
        auto sl = line.split('\t');
        if (n > 0 && sl.size() == 5)
        {
            SignalsDefines::StabRec rec(std::make_tuple(sl.at(2).toDouble(), sl.at(3).toDouble()));
            sigStab->addValue(rec);

            sigZ->addValue(sl.at(4).toDouble());
        }
        ++n;
    }
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
            addBilatChannel();
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

    ui->wgtAdvChannels->saveProbe();

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

        if (QApplication::desktop()->screenCount() > 1)
            showPatientWindow(m_params.at(m_probe).stimulCode);
        ui->cbScale->setCurrentIndex(m_params.at(m_probe).scale);

        ui->btnZeroing->setVisible(m_params.at(m_probe).zeroingEnabled);
        ui->btnZeroing->setEnabled(!m_isRecording);
        ui->btnCalibrate->setEnabled(!m_isRecording);
        ui->frScale->setEnabled(!m_isRecording);
        ui->wgtAdvChannels->enabledControls(!m_isRecording);

        scaleChange(ui->cbScale->currentIndex());
        m_stageNum = 0;
        m_autoModeSecCounter = 0;
    }
}

void StabTestExecute::finishTest()
{
    if (!ui->lblCommunicationError->isVisible())
        killTimer(m_autoModeTimerId);

    hidePatientWindow();
    m_isRecording = false;
    m_trd->saveTest();
    static_cast<ExecuteWidget*>(parent())->closeExecutePage(); //showDB();
}

void StabTestExecute::addBilatChannel()
{
    if (m_bilatControl)
    {
        auto rdBilat = new BilateralResultData(ChannelsDefines::chanBilat);
        rdBilat->addPlatform(m_platform1);
        rdBilat->addPlatform(m_platform2);
        m_trd->addChannel(rdBilat);
    }
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
    case 4:
        m_patientWin = new FiveZonesWindow(this);
        break;
    case 5:
        m_patientWin = new LinesWindow(this);
        break;
    default:
        m_patientWin = nullptr;
        break;
    }

    if (m_patientWin)
    {
        auto rect = static_cast<AAnalyserApplication*>(QApplication::instance())->getPatientWindowGeometry();
        m_patientWin->resize(rect.size());
        m_patientWin->move(rect.x(), rect.y());

        m_patientWin->setParams(m_params.at(m_probe).stimParams);
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

QString StabTestExecute::msgWaitEvent(const QString &eventName, const int sec) const
{
    return eventName + "\n" + QString::number(sec) +  "\n" + tr("секунд");
}

void StabTestExecute::setFrontComment(const QString &comment)
{
    ui->lblFrontComment->setText(comment);
    ui->lblFrontComment->setVisible(comment != "");
}

