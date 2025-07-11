#include "trenexecute.h"
#include "ui_trenexecute.h"


#include <QTimer>
#include <QDesktopWidget>
#include <QUuid>
#include <QDebug>

#include "aanalyserapplication.h"
#include "driver.h"
#include "executewidget.h"
#include "baseutils.h"
#include "channelsutils.h"
#include "testresultdata.h"
#include "trenresultdata.h"
#include "trenresultfactors.h"
#include "trenagerpatientwindow.h"
#include "settingsprovider.h"
#include "videoirritant.h"
#include "frontcommentitem.h"
#include "bilateralresultdata.h"
#include "aanalysersettings.h"
#include "metodicsfactory.h"
#include "amessagebox.h"

TrenExecute::TrenExecute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrenExecute)
  , m_scene(new QGraphicsScene(-1000, -1000, 2000, 2000))
  , m_trd(new TestResultData())
  , m_rdBilat(new BilateralResultData(ChannelsDefines::chanBilat))
{
    ui->setupUi(this);

    ui->lblCommunicationError->setVisible(false);
    ui->wgtAdvChannels->setVisible(false);

    m_kard = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentPatient();
    MetodicDefines::MetodicInfo mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentMetodic();
    ui->lblTrenTitle->setText(mi.name + " - " + m_kard.fio);
}

TrenExecute::~TrenExecute()
{
    delete ui;
}

void TrenExecute::setParams(const QJsonObject &params)
{
    m_recLength = params["time"].toInt();
    auto sRL = BaseUtils::getTimeBySecCount(m_recLength);
    ui->lblRecLenTitle->setText(QString(tr("Длительность записи") + " %1 " + tr("мин:сек")).arg(sRL));

    m_backgroundObj = params["background"].toObject();

    auto objPhisioChan = params["phisio_chan"].toObject();
    m_isPhisioChannel = objPhisioChan["enabled"].toBool();
    m_boundForce = objPhisioChan["force"].toInt(10);
    m_boundMyogram = objPhisioChan["myogram"].toInt(200);

    m_scoresPerMinute = params["scores_per_min"].toInt();
    m_isVisibleLabelScores = params["label_scores_visible"].toBool(true);

    QTimer::singleShot(0, this, &TrenExecute::start);
}

void TrenExecute::closeEvent(QCloseEvent *event)
{
    hidePatientWindow();

    //! Переехало из деструктора. Подозрение на нерегулярный сбой.
    //! Но для срабатывания необходимо перед delete вызывать close();
    doneDriver();
    QWidget::closeEvent(event);
}

void TrenExecute::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    QSize size = ui->gvGame->geometry().size();
    if (m_patientWindow && QApplication::desktop()->screenCount() > 1)
        size = m_patientWindow->sceneSize();
    setSceneSize(size);
}

void TrenExecute::timerEvent(QTimerEvent *event)
{
    if (!ui->lblCommunicationError->isVisible() && event->timerId() == m_autoModeTimerId)
    {
        if (m_stageNum < m_stages.size())
        {
            ++m_autoModeSecCounter;
            auto stage = m_stages.at(m_stageNum);
            auto stageTitle = MetodicsFactory::autoModeStageTitle(stage);

            if (stageTitle == "")
            {
                if (m_autoModeSecCounter == m_autoTimeLatent)
                {
                    ++m_stageNum;
                    if (m_stageNum < m_stages.size())
                    {
                        auto stage = m_stages.at(m_stageNum);
                        autoModeStageChanged(stage);
                        auto stageTitle = MetodicsFactory::autoModeStageTitle(stage);
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
                        doZeroing();
                    else
                    if (stage == MetodicDefines::amssRecordingWait)
                        on_recording();
                    ++m_stageNum;
                    autoModeStageChanged(m_stages.at(m_stageNum));
                    m_autoModeSecCounter = 0;
                }
            }
        }
    }

    QWidget::timerEvent(event);
}

void TrenExecute::start()
{
    m_driver = static_cast<AAnalyserApplication*>(QApplication::instance())->
            getDriverByFormats(QStringList() << ChannelsDefines::cfDecartCoordinates);
    if (m_driver)
    {
        m_bilatControl = static_cast<DeviceProtocols::MultiPlatformControl*>(m_driver->getDeviceControl(DeviceProtocols::uid_MultiPlatformControl));
        if (m_bilatControl)
        {
            m_platform1 = m_bilatControl->platform(0);
            m_platform2 = m_bilatControl->platform(1);
        }

        connect(m_driver, &Driver::sendData, this, &TrenExecute::getData);
        connect(m_driver, &Driver::communicationError, this, &TrenExecute::on_communicationError);

        m_kard = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentPatient();
        MetodicDefines::MetodicInfo mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentMetodic();
        m_trd->newTest(m_kard.uid, mi.uid);

        //! По формату получаем список каналов этого формата, которые передает драйвер, заносим их в список для выбора
        setMainChannels();
        setAdvancedChannels();

        auto chanUid = ui->cbSelectChannel->currentData(ChannelsUtils::ChannelUidRole).toString();
        m_frequency = m_driver->frequency(chanUid);

        ui->wgtAdvChannels->assignDriver(m_driver, m_trd);
        m_trd->newProbe(mi.name);
        connect(m_driver, &Driver::started, this, &TrenExecute::on_started);
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

        showPatientWindow();
        QTimer::singleShot(0, [=]   // Без singleShot другие размеры
        {
            generateNewScene();
            autoModeStageChanged(MetodicDefines::amssLatent0); //m_stages.at(m_stageNum));
        });

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
        static_cast<ExecuteWidget*>(parent())->showDB();
    }

    fillGameControl(ui->frGameControl);
    fillGameParams(ui->frGameParams);
    fillGameHints(ui->frGameHints);
}

void TrenExecute::getData(DeviceProtocols::DeviceData *data)
{
    //! Любой канал от драйвера надо передавать окну дополнительных каналов
    ui->wgtAdvChannels->getData(data);
    if (m_isRecording)
    {
        ui->wgtAdvChannels->record(data);
    }

    if (ui->cbSelectChannel->currentData(ChannelsUtils::ChannelUidRole).toString() == data->channelId())
    {
        ui->lblCommunicationError->setVisible(false);

        auto* multiData = static_cast<DeviceProtocols::MultiData*>(data);
        if (multiData->subChanCount() > 0)
        {
            //! Взаимодействие элементов
            elementsInteraction(data);

            if (m_isRecording)
            {
                ++m_recCount;
                ui->lblRecLen->setText(BaseUtils::getTimeBySecCount(m_recCount / m_frequency));
                ui->pbRec->setValue(100 * m_recCount / (m_recLength * m_frequency));
                if (m_recCount == m_recLength * m_frequency)
                {
                    initFinishTest();
                    return;
                }
                if (m_isFinishTest)
                {
                    finishTest();
                    return;
                }
            }

            m_scene->update(m_scene->sceneRect());
        }
    }

    if (ui->cbSelectAdvChannel->currentData(ChannelsUtils::ChannelUidRole).toString() == data->channelId())
    {
        auto* multiData = static_cast<DeviceProtocols::MultiData*>(data);
        if (multiData->subChanCount() > 0)
            m_adv0Value = qvariant_cast<double>(multiData->value(0));
    }

    if (ui->cbSelectAdvChannel_2->currentData(ChannelsUtils::ChannelUidRole).toString() == data->channelId())
    {
        auto* multiData = static_cast<DeviceProtocols::MultiData*>(data);
        if (multiData->subChanCount() > 0)
            m_adv1Value = qvariant_cast<double>(multiData->value(0));
    }
}

void TrenExecute::on_started()
{
    ui->wgtAdvChannels->newProbe();
}

QString TrenExecute::msgWaitEvent(const QString &eventName, const int sec) const
{
    return eventName + "\n" + QString::number(sec) +  "\n" + tr("секунд");
}

void TrenExecute::setFrontComment(const QString &comment)
{
    m_frontComment->setText(comment);
}

void TrenExecute::on_communicationError(const QString &drvName, const QString &port, const int errorCode)
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

void TrenExecute::on_selectChannel(int chanIdx)
{
    Q_UNUSED(chanIdx);
}

void TrenExecute::on_recording()
{
    m_isRecording = ! m_isRecording;

    ui->pbRec->setValue(0);
    ui->lblRecLen->setText("00:00");

    ui->wgtAdvChannels->enabledControls(!m_isRecording);

    if (m_isRecording)
    {
        ui->btnRecord->setIcon(QIcon(":/images/SaveNO.png"));
        ui->btnRecord->setText(tr("Прервать"));

//        MetodicDefines::MetodicInfo mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedMetodic();
//        m_trd->newProbe(mi.name);
//        ui->wgtAdvChannels->newProbe();  Не сбрасывает запись каналов
    }
    else
    {
        ui->btnRecord->setIcon(QIcon(":/images/Save.png"));
        ui->btnRecord->setText(tr("Запись"));

        //! Проба была прервана - очистить сигналы
        ui->wgtAdvChannels->abortProbe();
    }

    m_recCount = 0;
    m_gameScore = 0;
    changeGameScore(0);
}

void TrenExecute::on_advChannelsClicked(bool checked)
{
    ui->wgtAdvChannels->setVisible(checked);
}

void TrenExecute::on_selectAdvChannelClicked(int index)
{
    if (sender() == ui->cbSelectAdvChannel)
    {
        if (m_AdvChannel0Select)
            m_AdvChannel0Select->set(index);
    }
    else
    if (sender() == ui->cbSelectAdvChannel_2)
    {
        if (m_AdvChannel1Select)
            m_AdvChannel1Select->set(index);
    }
}

void TrenExecute::on_enabledAdvChannelChanged(bool enabled)
{
    if (sender() == ui->chbSelectAdvChannel)
    {
        if (m_AdvChannel0Enabled)
            m_AdvChannel0Enabled->set(enabled);
    }
    else
    if (sender() == ui->chbSelectAdvChannel_2)
    {
        if (m_AdvChannel1Enabled)
            m_AdvChannel1Enabled->set(enabled);
    }
}

void TrenExecute::on_selectIrriant(int idx)
{
    if (idx >= 0)
    {
        m_videoIrritant->setCurrentIrriant(idx - 1);

        //! Очистка панели параметров видеораздражителя
        while (QLayoutItem* item = ui->wgtIrriantsParams->layout()->takeAt(0))
        {
            delete item->widget();
            delete item;
        }

        //! Добавление виджета управления видеораздражителем
        if (idx > 0)
        {
            auto widget = m_videoIrritant->irriant(idx - 1)->getSettingsWidget();
            ui->wgtIrriantsParams->layout()->addWidget(widget);
        }
    }
    ui->sldTransparent->setEnabled(idx > 0);
}

void TrenExecute::on_changeTransparent(int value)
{
    m_videoIrritant->setTransparent(static_cast<double>(value) / static_cast<double>(100));
}

void TrenExecute::autoModeStageChanged(const MetodicDefines::AutoModeStaticStages stage)
{
    Q_UNUSED(stage);
}

void TrenExecute::setSceneSize(QSize &size)
{
    int sideSize = size.height();
    if (size.width() < size.height())
        sideSize = size.width();
    m_prop = static_cast<double>(sideSize) / 2000;
    if (m_scene)
        m_scene->setSceneRect(-size.width() * 0.995 / 2, - size.height() * 0.995 / 2, size.width() * 0.995, size.height() * 0.995);
    m_propX = static_cast<double>(size.width()) / 2000;
    m_propY = static_cast<double>(size.height()) / 2000;
}

void TrenExecute::generateNewScene()
{
    m_scene->clear();

    setBackground(m_backgroundObj);
    m_background->setData(edKindElement, ekBackground);
    m_scene->addItem(m_background);

    setVideoIrritant();
    m_videoIrritant->setData(edKindElement, ekIrriant);
    m_scene->addItem(m_videoIrritant);

    setFrontCommentItem();
    m_frontComment->setText("");
    m_frontComment->setData(edKindElement, ekFrontComment);
    m_scene->addItem(m_frontComment);
}

void TrenExecute::elementsInteraction(DeviceProtocols::DeviceData *data)
{
    Q_UNUSED(data);
}

void TrenExecute::setMainChannels()
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

void TrenExecute::setAdvancedChannels()
{
    auto listChanUid = m_driver->getChannelsByFormat(ChannelsDefines::cfSinglePositive);
    foreach (auto channelUid, listChanUid)
    {
        auto subChanCnt = m_driver->getSubChannelsCount(channelUid);

        for (int i = 0; i < subChanCnt; ++i)
        {
            auto name = ChannelsUtils::instance().channelName(channelUid);
            if (subChanCnt > 1)
                name += (" " + QString::number(i + 1));
            ui->cbSelectAdvChannel->addItem(name);
            ui->cbSelectAdvChannel->setItemData(ui->cbSelectAdvChannel->count() - 1, channelUid, ChannelsUtils::ChannelUidRole);
            ui->cbSelectAdvChannel->setItemData(ui->cbSelectAdvChannel->count() - 1, i, ChannelsUtils::SubChanNumRole);
            ui->cbSelectAdvChannel_2->addItem(name);
            ui->cbSelectAdvChannel_2->setItemData(ui->cbSelectAdvChannel_2->count() - 1, channelUid, ChannelsUtils::ChannelUidRole);
            ui->cbSelectAdvChannel_2->setItemData(ui->cbSelectAdvChannel_2->count() - 1, i, ChannelsUtils::SubChanNumRole);
        }
    }
    ui->frAdvChannels->setVisible(m_isPhisioChannel);
    ui->frSelectAdvChannel->setVisible(m_isPhisioChannel);
    ui->frSelectAdvChannel_2->setVisible(m_isPhisioChannel);

    //! Установка дефолтных значений
    m_AdvChannel0Select = new SettingsValue("SelectionValues/" + getAutoSaveParamsSectionName(), "AdvChannel0", 0);
    m_AdvChannel1Select = new SettingsValue("SelectionValues/" + getAutoSaveParamsSectionName(), "AdvChannel1", 1);
    if (ui->cbSelectAdvChannel->count() > m_AdvChannel0Select->value().toInt())
        ui->cbSelectAdvChannel->setCurrentIndex(m_AdvChannel0Select->value().toInt());
    if (ui->cbSelectAdvChannel_2->count() > m_AdvChannel1Select->value().toInt())
        ui->cbSelectAdvChannel_2->setCurrentIndex(m_AdvChannel1Select->value().toInt());

    m_AdvChannel0Enabled = new SettingsValue("SelectionValues/" + getAutoSaveParamsSectionName(), "AdvChannel0Enabled", true);
    m_AdvChannel1Enabled = new SettingsValue("SelectionValues/" + getAutoSaveParamsSectionName(), "AdvChannel1Enabled", true);
    ui->chbSelectAdvChannel->setChecked(m_AdvChannel0Enabled->value().toBool());
    ui->chbSelectAdvChannel_2->setChecked(m_AdvChannel1Enabled->value().toBool());
}

void TrenExecute::setAdvancedChannelEnable(const int chan, const bool enable)
{
    //! Глобальное включение. И именно так, ui->frAdvChannels->setVisible(enable) выключит, если выключим для одного канала, а для другого может быть включен
    if (enable)
        ui->frAdvChannels->setVisible(true);

    if (chan == 0)
        ui->frSelectAdvChannel->setVisible(enable);
    else
    if (chan == 1)
        ui->frSelectAdvChannel_2->setVisible(enable);
}

void TrenExecute::setAdvancedFunctionTitle(const int chan, const QString title)
{
    if (chan == 0)
        ui->chbSelectAdvChannel->setText(title);
    else
    if (chan == 1)
        ui->chbSelectAdvChannel_2->setText(title);
}

void TrenExecute::showPatientWindow()
{
    auto winPresent = SettingsProvider::valueFromRegAppCopy("", "PatientWindow", static_cast<QVariant>(true)).toBool();

    if (winPresent && QApplication::desktop()->screenCount() > 1)
    {
        if (!m_patientWindow)
            m_patientWindow = new TrenagerPatientWindow(this);
        m_patientWindow->setScene(m_scene);
        QSize size = m_patientWindow->sceneSize();
        setSceneSize(size);
        auto rect = static_cast<AAnalyserApplication*>(QApplication::instance())->getPatientWindowGeometry();
        m_patientWindow->resize(rect.size());
        m_patientWindow->move(rect.x(), rect.y());
        m_patientWindow->show();
        m_prop = m_patientWindow->prop();
        m_propX = m_patientWindow->propX();
        m_propY = m_patientWindow->propY();
    }
    else
    {
        ui->gvGame->setScene(m_scene);
        QSize size = ui->gvGame->geometry().size();
        setSceneSize(size);
    }
}

void TrenExecute::hidePatientWindow()
{
    if (m_patientWindow)
    {
        m_patientWindow->hide();
        auto* p = m_patientWindow;
        m_patientWindow = nullptr;
        delete p;
    }
}

void TrenExecute::setBackground(const QJsonObject &objBackground)
{
    auto style = objBackground["style"].toString();
    if (style == "picture")
    {
        m_background = new GraphicCommon::BackgroundElement(GraphicCommon::bkgmPicture);
        m_background->assignPixmap(":/images/backgrounds/" + objBackground["image"].toString());
        m_background->setRect(m_scene->sceneRect());
        m_background->setZValue(zlvlBackground);
    }
    else
    if (style == "plate")
    {
        m_background = new GraphicCommon::BackgroundElement(GraphicCommon::bkgmPlate);
        m_background->assignPixmap(":/images/plite_textures/" + objBackground["image"].toString());
        m_background->setRect(m_scene->sceneRect());
        m_background->setZValue(zlvlBackground);
    }
    auto bo = objBackground["borders"].toObject();
    m_bndTop = bo["top"].toInt();
    m_bndBottom = bo["bottom"].toInt();
    m_bndLeft = bo["left"].toInt();
    m_bndRight = bo["right"].toInt();
}

void TrenExecute::setVideoIrritant()
{
    m_videoIrritant = new VideoIrritant(m_scene->sceneRect());
    m_videoIrritant->setZValue(zlvlVideoIrritant);
    m_videoIrritant->setTransparent(static_cast<double>(ui->sldTransparent->value()) / static_cast<double>(100));

    ui->cbIrriants->clear();
    ui->cbIrriants->addItem("<" + tr("нет") + ">");
    for (int i = 0; i < m_videoIrritant->irriantsCount(); ++i)
    {
        ui->cbIrriants->addItem(m_videoIrritant->irriant(i)->name());
    }
}

void TrenExecute::setFrontCommentItem()
{
    m_frontComment = new FrontCommentItem(m_scene->sceneRect());
    m_frontComment->setZValue(zlvlFrontComment);

}

void TrenExecute::finishTest()
{
    ui->wgtAdvChannels->saveProbe();

    m_isClosed = true;
    doneDriver();
    auto trenRes = new TrenResultData(ChannelsDefines::chanTrenResult);
    trenRes->addFactor(TrenResultFactorsDefines::ScoreUid, m_gameScore);
    double sf = (static_cast<double>(m_gameScore) / static_cast<double>(m_recLength) * 60.0) / static_cast<int>(m_scoresPerMinute) * 100;
    if (sf > 100)
        sf = 100;
    if (sf < 0)
        sf = 0;
    trenRes->addFactor(FactorsDefines::CommonFactors::SuccessUid, sf);
    foreach (auto fct, m_gameFactors)
        trenRes->addFactor(fct.uid, fct.value);
    m_trd->addChannel(trenRes);

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

void TrenExecute::initFinishTest()
{
    finishTest();
}

void TrenExecute::doneFinishTest()
{
    m_isFinishTest = true;
}

void TrenExecute::doneDriver()
{
    if (m_driver)
    {
        m_driver->stop();
        m_driver->deleteLater();
    }
}

void TrenExecute::fillGameControl(QFrame *frame)
{
    Q_UNUSED(frame);
}

void TrenExecute::fillGameHints(QFrame *frame)
{
    Q_UNUSED(frame);
}

void TrenExecute::fillGameParams(QFrame *frame)
{
    QString style = "font-size: 18pt; font-weight: 900; color: rgb(0,150,0);";
    QString name = tr("Баллы");
    m_lblGameScore = new QLabel(frame);
    m_lblGameScore->setText(name);
    m_lblGameScore->setStyleSheet(style);
    frame->layout()->addWidget(m_lblGameScore);
    if (m_patientWindow && QApplication::desktop()->screenCount() > 1)
        m_patientWindow->setGameParamLabel(name, style, m_isVisibleLabelScores);
    m_lblGameScore->setVisible(m_isVisibleLabelScores);

    changeGameScore(0);
}

QString TrenExecute::currentChannelUID()
{
    if (ui->cbSelectChannel->count() > 0)
        return ui->cbSelectChannel->currentData(ChannelsUtils::ChannelUidRole).toString();
    return "";
}

int TrenExecute::getFrameControlWidth()
{
    return ui->frControl->width();
}

int TrenExecute::pwGetFrameParamsWidth()
{
    if (m_patientWindow && QApplication::desktop()->screenCount() > 1)
        return m_patientWindow->getFrameParamsWidth();
    return 0;
}

void TrenExecute::changeGameScore(const int value)
{
    m_gameScore += value;
    if (m_gameScore < 0)
        m_gameScore = 0;
    QString text = tr("Баллы") + " - " + QString::number(m_gameScore);
    m_lblGameScore->setText(text);

    if (m_patientWindow && QApplication::desktop()->screenCount() > 1)
        m_patientWindow->setGameParamLabelValue(0, text);
}

void TrenExecute::pwSetGameParamLabel(const QString text, const QString styleSheet, const bool isVisible)
{
    if (m_patientWindow && QApplication::desktop()->screenCount() > 1)
        m_patientWindow->setGameParamLabel(text, styleSheet, isVisible);
}

void TrenExecute::pwSetGameParamLabelValue(const int idxParam, const QString value)
{
    if (m_patientWindow && QApplication::desktop()->screenCount() > 1)
        m_patientWindow->setGameParamLabelValue(idxParam, value);
}

void TrenExecute::pwAddHintWidget(QWidget *widget)
{
    if (m_patientWindow && QApplication::desktop()->screenCount() > 1)
        m_patientWindow->addHintWidget(widget);
}

void TrenExecute::addFactorValue(const QString &uid, const double value)
{
    GameFactors fct;
    fct.uid = uid;
    fct.value = value;
    m_gameFactors << fct;
}

double TrenExecute::advancedValue(const int chan) const
{
    if (chan == 0)
        return m_adv0Value;
    else
    if (chan == 1)
        return m_adv1Value;
    return 0;
}

bool TrenExecute::isAdvancedChannelAboveBound(const int chan, const bool isCommonAllowing)
{
    auto isAboveBound = [&](QComboBox* comboBox, double& value)
    {
        auto chanId = comboBox->currentData(ChannelsUtils::ChannelUidRole).toString();
        auto chanType = ChannelsUtils::instance().channelType(chanId);
        if (chanType == ChannelsDefines::ctDynamo)
            return value > m_boundForce;
        else
        if (chanType == ChannelsDefines::ctMyogram)
            return value > m_boundMyogram;
        return true;
    };

    if ((isCommonAllowing && m_isPhisioChannel) || (!isCommonAllowing))
   {
       if (chan == 0)
       {
           if (ui->chbSelectAdvChannel->isChecked())
               return isAboveBound(ui->cbSelectAdvChannel, m_adv0Value);
       }
       else
           if (ui->chbSelectAdvChannel_2->isChecked())
               return isAboveBound(ui->cbSelectAdvChannel_2, m_adv1Value);
   }

   return true;
}

bool TrenExecute::isAdvancedChannelAboveBoundNow(const int chan, const bool isCommonAllowing)
{
    if ((isCommonAllowing && m_isPhisioChannel) || (!isCommonAllowing))
    {
        auto above = isAdvancedChannelAboveBound(chan, isCommonAllowing);
        bool retval = false;
        if (chan == 0)
        {
            if (ui->chbSelectAdvChannel->isChecked() && above && !m_isAdv0ChannelAboveBound)
                retval = true;
            m_isAdv0ChannelAboveBound = above;
        }
        else
        if (chan == 1)
        {
            if (ui->chbSelectAdvChannel_2->isChecked() && above && !m_isAdv1ChannelAboveBound)
                retval = true;
            m_isAdv1ChannelAboveBound = above;
        }
        return retval;
    }
    else
        return false;

}

void TrenExecute::addChannel(ChannelData *channel)
{
    if (m_trd)
        m_trd->addChannel(channel);
}

void TrenExecute::doZeroing()
{

}
