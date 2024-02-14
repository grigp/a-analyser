#include "stabdynamictestexecute.h"
#include "ui_stabdynamictestexecute.h"

#include "testresultdata.h"
#include "aanalyserapplication.h"
#include "driver.h"
#include "channelsutils.h"
#include "settingsprovider.h"
#include "executewidget.h"
#include "stabdynamictestpatientwindow.h"
#include "baseutils.h"
#include "bilateralresultdata.h"
#include "aanalysersettings.h"

#include <QTimer>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QDebug>

StabDynamicTestExecute::StabDynamicTestExecute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StabDynamicTestExecute)
  , m_trd(new TestResultData())
  , m_rdBilat(new BilateralResultData(ChannelsDefines::chanBilat))
{
    ui->setupUi(this);

    ui->lblCommunicationError->setVisible(false);
    QTimer::singleShot(0, this, &StabDynamicTestExecute::start);

    ui->cbScale->addItem("1");
    ui->cbScale->addItem("2");
    ui->cbScale->addItem("4");
    ui->cbScale->addItem("8");
    ui->cbScale->addItem("16");
    ui->cbScale->addItem("32");
    ui->cbScale->addItem("64");
    ui->cbScale->addItem("128");
    QTimer::singleShot(100, [=]
    {
        scaleChange(ui->cbScale->currentIndex());
    });

    ui->wgtAdvChannels->setVisible(false);
    ui->btnCalibrate->setVisible(false);
    ui->btnRecord->setEnabled(false);

    ui->lblFrontComment->setStyleSheet(MetodicDefines::AutoModeMessageStyleMain);
    setFrontComment("");
    m_stages = getAutoModeStaticStages();
}

StabDynamicTestExecute::~StabDynamicTestExecute()
{
    delete ui;
}

void StabDynamicTestExecute::setParams(const QJsonObject &params)
{
    Q_UNUSED(params);
}

void StabDynamicTestExecute::closeEvent(QCloseEvent *event)
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

void StabDynamicTestExecute::timerEvent(QTimerEvent *event)
{
    if (!ui->lblCommunicationError->isVisible() && event->timerId() == m_autoModeTimerId)
    {
        if (m_stageNum < m_stages.size())
        {
            ++m_autoModeSecCounter;
            auto stage = m_stages.at(m_stageNum);
            auto stageTitle = MetodicDefines::AutoModeStageTitle.value(stage);

            if (stageTitle == "")
            {
                if (m_autoModeSecCounter == m_autoTimeLatent)
                {
                    ++m_stageNum;
                    if (m_stageNum < m_stages.size())
                    {
                        auto stage = m_stages.at(m_stageNum);
                        auto stageTitle = MetodicDefines::AutoModeStageTitle.value(stage);
                        setFrontComment(msgWaitEvent(stageTitle, m_autoTimeRun));
                    }
                    m_autoModeSecCounter = 0;
                }
            }
            else
            {
                setFrontComment(msgWaitEvent(stageTitle, m_autoTimeRun - m_autoModeSecCounter));
                if (m_autoModeSecCounter == m_autoTimeRun)
                {
                    setFrontComment("");
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

void StabDynamicTestExecute::resizeEvent(QResizeEvent *event)
{
    ui->lblFrontComment->setGeometry(ui->wgtSKG->geometry());
    QWidget::resizeEvent(event);
}

void StabDynamicTestExecute::setTitle(const QString &title)
{
    ui->lblProbeTitle->setText(title);
}

int StabDynamicTestExecute::diap() const
{
    int v = 1;
    for (int i = 0; i < ui->cbScale->currentIndex(); ++i)
        v = v * 2;
    return  128 / v;
}

void StabDynamicTestExecute::isShowValues(const bool isShow)
{
    ui->frValues->setVisible(isShow);
}

void StabDynamicTestExecute::isTraceControl(const bool isTrace)
{
    ui->cbShowTrace->setVisible(isTrace);
}

void StabDynamicTestExecute::setRecordLengthTitle(const QString title)
{
    ui->lblRecLenTitle->setText(title);
}

void StabDynamicTestExecute::setVisibleRecordLength(const bool visible)
{
    ui->lblRecLenTitle->setVisible(visible);
    ui->lblRecLen->setVisible(visible);
    ui->pbRec->setVisible(visible);
}

void StabDynamicTestExecute::setRecordLength(const int length)
{
    m_recLength = length;
    ui->lblRecLen->setText(BaseUtils::getTimeBySecCount(m_recLength / m_freqStab));

}

void StabDynamicTestExecute::setRecordPosition(const int pos, const int recLen)
{
    if (recLen > 0)
    {
        ui->lblRecLen->setText(BaseUtils::getTimeBySecCount(pos / m_freqStab) + " / " +
                               BaseUtils::getTimeBySecCount(recLen / m_freqStab));
        ui->pbRec->setValue( pos * 100 / recLen);
    }
}

QString StabDynamicTestExecute::selectedChannel() const
{
    return ui->cbSelectChannel->currentData(ChannelsUtils::ChannelUidRole).toString();
}

void StabDynamicTestExecute::addMarker()
{
    ui->wgtSKG->addMarker();
}

void StabDynamicTestExecute::setVisibleMarker(const bool isVisible)
{
    ui->wgtSKG->setVisibleMarker(isVisible);
}

void StabDynamicTestExecute::addTarget(const double x, const double y, const QColor colorBackground, const QColor colorBorder)
{
    ui->wgtSKG->addTarget(x, y, colorBackground, colorBorder);
}

void StabDynamicTestExecute::setTarget(const double x, const double y, const int idx)
{
    ui->wgtSKG->setTarget(x, y, idx);
    if (m_patientWin)
        m_patientWin->setTarget(x, y, idx);
}

void StabDynamicTestExecute::clearTargets()
{
    ui->wgtSKG->clearTargets();
    if (m_patientWin)
        m_patientWin->clearTargets();
}

StabDynamicTestPatientWindow *StabDynamicTestExecute::createPatientWindow()
{
    return nullptr;
}

void StabDynamicTestExecute::finishTest()
{
    ui->wgtAdvChannels->saveProbe();

    hidePatientWindow();
    m_isRecording = false;
    if (m_bilatControl)
    {
        m_rdBilat->addPlatform(m_platform1);
        m_rdBilat->addPlatform(m_platform2);
        m_trd->addChannel(m_rdBilat);
    }
    m_isRecording = false;
    m_trd->saveTest();
    static_cast<ExecuteWidget*>(parent())->closeExecutePage(); //showDB();
}

void StabDynamicTestExecute::fillSpecific(QFrame *frSpecific)
{
    Q_UNUSED(frSpecific);
}

QList<MetodicDefines::AutoModeStaticStages> StabDynamicTestExecute::getAutoModeStaticStages()
{
    return MetodicDefines::AutoStagesBase;
}

void StabDynamicTestExecute::start()
{
    fillSpecific(ui->frSpecific);

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
        }

        connect(m_driver, &Driver::sendData, this, &StabDynamicTestExecute::getData);
        connect(m_driver, &Driver::communicationError, this, &StabDynamicTestExecute::on_communicationError);

        setChannels();

        m_kard = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentPatient();
        m_metInfo = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentMetodic();
        m_trd->newTest(m_kard.uid, m_metInfo.uid);

        if (QApplication::desktop()->screenCount() > 1)
            createAndShowPatientWindow();
        ui->cbScale->setCurrentIndex(0);
        QTimer::singleShot(100, [&]  //! Пока процесс создания не завершен, масштаб отображается некорректно, если просто вызывать
        {
            scaleChange(ui->cbScale->currentIndex());
        });

        ui->wgtAdvChannels->assignDriver(m_driver, m_trd);
        m_trd->newProbe(m_metInfo.name);
        connect(m_driver, &Driver::started, this, &StabDynamicTestExecute::on_started);

        //! Стабилограмма будет записана всегда
        ui->wgtAdvChannels->setAllwaysRecordingChannel(ui->cbSelectChannel->currentData(ChannelsUtils::ChannelUidRole).toString());
        if (m_bilatControl)
        {
            ui->wgtAdvChannels->setAllwaysRecordingChannel(ChannelsDefines::chanStabLeft);
            ui->wgtAdvChannels->setAllwaysRecordingChannel(ChannelsDefines::chanStabRight);
        }

        auto val = SettingsProvider::valueFromRegAppCopy("AdvancedChannelsWidget", "SplitterProbePosition").toByteArray();
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
        QMessageBox::warning(this, tr("Предупреждение"), tr("Отсутствует необходимое подключение для работы теста"));
        static_cast<ExecuteWidget*>(parent())->showDB();
    }
}

void StabDynamicTestExecute::getData(DeviceProtocols::DeviceData *data)
{
    //! Любой канал от драйвера надо передавать окну дополнительных каналов
    ui->wgtAdvChannels->getData(data);
    //! Запись, если она запущена и нет паузы
    if (m_isRecording && !m_isPause)
        ui->wgtAdvChannels->record(data);

    //! Выбранный в переключателе канал, а не просто данные.
    //! Если драйвер будет передавать несколько стабилограмм, то отображать здесь мы будем только одну
    if (ui->cbSelectChannel->currentData(ChannelsUtils::ChannelUidRole).toString() == data->channelId())
    {
        ui->lblCommunicationError->setVisible(false);

        DeviceProtocols::StabDvcData *stabData = static_cast<DeviceProtocols::StabDvcData*>(data);
        m_x = stabData->x();
        m_y = stabData->y();
        m_z = stabData->z();
        ui->lblX->setText(QString("X = %1").arg(m_x, 0, 'f', 2));
        ui->lblY->setText(QString("Y = %1").arg(m_y, 0, 'f', 2));
        ui->lblZ->setText(QString("Z = %1").arg(m_z, 0, 'f', 2));
        ui->wgtSKG->setMarker(m_x, m_y);

        if (m_patientWin)
            m_patientWin->setMarker(m_x, m_y);


        if (m_isRecording)
        {
            ++m_recCount;

            if (m_recLength > 0)
            {
                ui->lblRecLen->setText(BaseUtils::getTimeBySecCount(m_recCount / m_freqStab) + " / " +
                                       BaseUtils::getTimeBySecCount(m_recLength / m_freqStab));
                ui->pbRec->setValue( m_recCount * 100 / m_recLength);
            }
        }

//        if (m_isRecording)
//        {
//            ++m_recCount;
//            //! Вывод времени теста и прогресса
//            if (probeParams().autoEnd)
//            {
//                double rc = m_recCount - probeParams().latentTime * m_freqStab;
//                if (rc < 0)
//                    rc = 0;
//                if (m_recCount < probeParams().latentTime * m_freqStab)
//                    ui->lblRecLen->setText(tr("Задержка привыкания"));
//                else
//                    ui->lblRecLen->setText(BaseUtils::getTimeBySecCount(rc / m_freqStab) + " / " +
//                                           BaseUtils::getTimeBySecCount(probeParams().time));
//                double mrc = probeParams().time * m_freqStab;
//                ui->pbRec->setValue(rc / mrc * 100);
//            }
//            else
//                ui->lblRecLen->setText(BaseUtils::getTimeBySecCount(m_recCount / m_freqStab));

//            //! Смена пробы и окончание
//            if (probeParams().autoEnd && (m_recCount >= (probeParams().time + probeParams().latentTime) * m_freqStab))
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

void StabDynamicTestExecute::on_communicationError(const QString &drvName, const QString &port, const int errorCode)
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

void StabDynamicTestExecute::recording()
{
    m_isRecording = ! m_isRecording;

    ui->pbRec->setValue(0);
    ui->lblRecLen->setText("00:00");

    ui->btnZeroing->setEnabled(!m_isRecording);
    ui->btnCalibrate->setEnabled(!m_isRecording);
    ui->frScale->setEnabled(!m_isRecording);
    ui->wgtAdvChannels->enabledControls(!m_isRecording);

    m_recCount = 0;

    if (m_isRecording)
    {
        if (isAutoFinishRecord())
        {
            ui->btnRecord->setIcon(QIcon(":/images/SaveNO.png"));
            ui->btnRecord->setText(tr("Прервать"));
        }
        else
        {
            ui->btnRecord->setIcon(QIcon(":/images/SaveOK.png"));
            ui->btnRecord->setText(tr("Завершить"));
        }

        if (QApplication::desktop()->screenCount() == 1)
        {
            createAndShowPatientWindow();
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

        if (isAutoFinishRecord())
            ui->wgtAdvChannels->abortProbe();
        else
        {
            finishTest();
        }
    }
}

void StabDynamicTestExecute::setPatientWinDiap(const int diap)
{
    if (m_patientWin)
        m_patientWin->setDiap(diap);
}

void StabDynamicTestExecute::setFrontComment(const QString &comment, const bool isPatientOnly)
{
    if (!isPatientOnly)
    {
        ui->lblFrontComment->setText(comment);
        ui->lblFrontComment->setVisible(comment != "");
    }
    if (m_patientWin)
        m_patientWin->setFrontComment(comment);
}

void StabDynamicTestExecute::on_started()
{
    ui->wgtAdvChannels->newProbe();
}

void StabDynamicTestExecute::scaleChange(int scaleId)
{
    int v = 1;
    for (int i = 0; i < scaleId; ++i)
        v = v * 2;
    ui->wgtSKG->setDiap(128 / v);
    setPatientWinDiap(128 / v);
}

void StabDynamicTestExecute::zeroing()
{
    ui->btnRecord->setEnabled(true);
    if (m_stabControl)
        m_stabControl->zeroing(ChannelsDefines::chanStab);
}

void StabDynamicTestExecute::calibrate()
{

}

void StabDynamicTestExecute::on_advChannelsClicked(bool checked)
{
    ui->wgtAdvChannels->setVisible(checked);
}

void StabDynamicTestExecute::splitterMoved(int pos, int index)
{
    Q_UNUSED(pos);
    Q_UNUSED(index);
}

void StabDynamicTestExecute::setChannels()
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

void StabDynamicTestExecute::createAndShowPatientWindow()
{
    m_patientWin = createPatientWindow();

    if (m_patientWin)
    {
        auto rect = static_cast<AAnalyserApplication*>(QApplication::instance())->getPatientWindowGeometry();
        m_patientWin->resize(rect.size());
        m_patientWin->move(rect.x(), rect.y());
        m_patientWin->show();
    }
}

void StabDynamicTestExecute::hidePatientWindow()
{
    if (m_patientWin)
    {
        StabDynamicTestPatientWindow *pw = m_patientWin;
        m_patientWin = nullptr;
        delete pw;
    }
}

QString StabDynamicTestExecute::msgWaitEvent(const QString &eventName, const int sec) const
{
    return eventName + "\n" + QString::number(sec) +  "\n" + tr("секунд");
}


