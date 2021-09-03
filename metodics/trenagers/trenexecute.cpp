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
        setChannels();

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
    if (ui->cbSelectChannel->currentData(ChannelsUtils::ChannelUidRole).toString() == data->channelId())
    {
        ui->lblCommunicationError->setVisible(false);

        auto* multiData = static_cast<DeviceProtocols::MultiData*>(data);
        if (multiData->subChanCount() > 0)
        {
            ui->wgtAdvChannels->getData(data);

            //! Взаимодействие элементов
            elementsInteraction(data);

            if (m_isRecording)
            {
                ++m_recCount;
                ui->wgtAdvChannels->record(data);
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

void TrenExecute::setChannels()
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
