#include "trenexecute.h"
#include "ui_trenexecute.h"


#include <QTimer>
#include <QDesktopWidget>
#include <QMessageBox>
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

TrenExecute::TrenExecute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrenExecute)
  , m_scene(new QGraphicsScene(-1000, -1000, 2000, 2000))
  , m_trd(new TestResultData())
{
    ui->setupUi(this);

    ui->lblCommunicationError->setVisible(false);
    ui->wgtAdvChannels->setVisible(false);
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
    m_boundForce = objPhisioChan["force"].toInt(20);
    m_boundMyogram = objPhisioChan["myogram"].toInt(200);

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

void TrenExecute::start()
{
    m_driver = static_cast<AAnalyserApplication*>(QApplication::instance())->
            getDriverByFormats(QStringList() << ChannelsDefines::cfDecartCoordinates);
    if (m_driver)
    {
        connect(m_driver, &Driver::sendData, this, &TrenExecute::getData);
        connect(m_driver, &Driver::communicationError, this, &TrenExecute::on_communicationError);

        m_kard = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedPatient();
        MetodicDefines::MetodicInfo mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedMetodic();
        m_trd->newTest(m_kard.uid, mi.uid);

        //! По формату получаем список каналов этого формата, которые передает драйвер, заносим их в список для выбора
        setMainChannels();
        setAdvancedChannels();

        auto chanUid = ui->cbSelectChannel->currentData(ChannelsUtils::ChannelUidRole).toString();
        m_frequency = m_driver->frequency(chanUid);

        ui->wgtAdvChannels->assignDriver(m_driver, m_trd);
        //! Стабилограмма будет записана всегда
        ui->wgtAdvChannels->setAllwaysRecordingChannel(ui->cbSelectChannel->currentData(ChannelsUtils::ChannelUidRole).toString());
        auto val = SettingsProvider::valueFromRegAppCopy("AdvancedChannelsWidget", "SplitterProbePosition").toByteArray();
        ui->splitter->restoreState(val);

        m_driver->start();

        showPatientWindow();
        QTimer::singleShot(0, [=]   // Без singleShot другие размеры
        {
            generateNewScene();
        });
    }
    else
    {
        QMessageBox::warning(this, tr("Предупреждение"), tr("Отсутствует необходимое подключение для работы теста"));
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
                if (m_recCount >= m_recLength * m_frequency)
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

void TrenExecute::on_communicationError(const QString &drvName, const QString &port, const int errorCode)
{
    Q_UNUSED(errorCode);
    ui->lblCommunicationError->setText(QString(tr("Ошибка связи с устройством") + ": %1 (" + tr("порт") + ": %2)").
                                       arg(drvName).arg(port));
    ui->lblCommunicationError->setVisible(true);
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

        MetodicDefines::MetodicInfo mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedMetodic();
        m_trd->newProbe(mi.name);
        ui->wgtAdvChannels->newProbe();
    }
    else
    {
        ui->btnRecord->setIcon(QIcon(":/images/Save.png"));
        ui->btnRecord->setText(tr("Запись"));

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
    m_scene->addItem(m_background);
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

    //! Установка дефолтных значений
    m_AdvChannel0Select = new SettingsValue("SelectionValues/TrenagerType1", "AdvChannel0", 0);
    m_AdvChannel1Select = new SettingsValue("SelectionValues/TrenagerType1", "AdvChannel1", 1);
    if (ui->cbSelectAdvChannel->count() > m_AdvChannel0Select->value().toInt())
        ui->cbSelectAdvChannel->setCurrentIndex(m_AdvChannel0Select->value().toInt());
    if (ui->cbSelectAdvChannel_2->count() > m_AdvChannel1Select->value().toInt())
        ui->cbSelectAdvChannel_2->setCurrentIndex(m_AdvChannel1Select->value().toInt());

    m_AdvChannel0Enabled = new SettingsValue("SelectionValues/TrenagerType1", "AdvChannel0Enabled", true);
    m_AdvChannel1Enabled = new SettingsValue("SelectionValues/TrenagerType1", "AdvChannel1Enabled", true);
    ui->chbSelectAdvChannel->setChecked(m_AdvChannel0Enabled->value().toBool());
    ui->chbSelectAdvChannel_2->setChecked(m_AdvChannel1Enabled->value().toBool());
}

void TrenExecute::setAdvancedChannelEnable(const int chan, const bool enable)
{
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

void TrenExecute::finishTest()
{
    m_isClosed = true;
    doneDriver();
    auto trenRes = new TrenResultData(ChannelsDefines::chanTrenResult);
    trenRes->addFactor(TrenResultFactorsDefines::ScoreUid, m_gameScore);
    foreach (auto fct, m_gameFactors)
        trenRes->addFactor(fct.uid, fct.value);
    m_trd->addChannel(trenRes);

    m_isRecording = false;
    m_trd->saveTest();
    static_cast<ExecuteWidget*>(parent())->showDB();
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
    QString name = tr("Очки");
    m_lblGameScore = new QLabel(frame);
    m_lblGameScore->setText(name);
    m_lblGameScore->setStyleSheet(style);
    frame->layout()->addWidget(m_lblGameScore);
    if (m_patientWindow && QApplication::desktop()->screenCount() > 1)
        m_patientWindow->setGameParamLabel(name, style);

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
    QString text = tr("Очки") + " - " + QString::number(m_gameScore);
    m_lblGameScore->setText(text);

    if (m_patientWindow && QApplication::desktop()->screenCount() > 1)
        m_patientWindow->setGameParamLabelValue(0, text);
}

void TrenExecute::pwSetGameParamLabel(const QString text, const QString styleSheet)
{
    if (m_patientWindow && QApplication::desktop()->screenCount() > 1)
        m_patientWindow->setGameParamLabel(text, styleSheet);
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

double TrenExecute::advancedValue(const int chan)
{
    if (chan == 0)
        return m_adv0Value;
    else
    if (chan == 1)
        return m_adv1Value;
    return 0;
}

bool TrenExecute::isAdvancedChannelAboveBound(const int chan)
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

   if (m_isPhisioChannel)
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

bool TrenExecute::isAdvancedChannelAboveBoundNow(const int chan)
{
    if (m_isPhisioChannel)
    {
        auto above = isAdvancedChannelAboveBound(chan);
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
