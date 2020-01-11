#include "trentakeputexecute.h"
#include "ui_trentakeputexecute.h"

#include "aanalyserapplication.h"
#include "driver.h"
#include "executewidget.h"
#include "baseutils.h"
#include "channelsutils.h"
#include "trentakeputpatientwindow.h"
#include "settingsprovider.h"
#include "testresultdata.h"

#include <QTimer>
#include <QMessageBox>
#include <QPointF>
#include <QDesktopWidget>
#include <QDir>
#include <QDebug>

//namespace  {

//QList<TrenTakePutDefines::GameElement*> takeElements;
//QList<TrenTakePutDefines::GameElement*> putElements;

//}

TrenTakePutExecute::TrenTakePutExecute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrenTakePutExecute)
  , m_scene(new QGraphicsScene(-1000, -1000, 2000, 2000))
  , m_trd(new TestResultData())
{
    ui->setupUi(this);

    ui->lblCommunicationError->setVisible(false);
//    QTimer::singleShot(0, this, &TrenTakePutExecute::start);

    ui->cbScale->addItem("1");
    ui->cbScale->addItem("2");
    ui->cbScale->addItem("4");
    ui->cbScale->addItem("8");
    ui->cbScale->addItem("16");
    ui->cbScale->addItem("32");
    ui->cbScale->addItem("64");
    ui->cbScale->addItem("128");

    m_filesUsed.clear();
    ui->wgtAdvChannels->setVisible(false);
}

TrenTakePutExecute::~TrenTakePutExecute()
{
    delete ui;
}

void TrenTakePutExecute::setParams(const QJsonObject &params)
{
    auto arrTakeZones = params["take_zones"].toArray();
    auto arrTakeElements = params["take_elements"].toArray();
    setZones(arrTakeZones, m_zonesTake);
    setElements(arrTakeElements, m_elementsTake, TrenTakePutDefines::gsTake);

    if ((m_elementsTake.size() == 1) && (m_elementsTake.at(0).style == TrenTakePutDefines::esPictureSplit))
        loadPicturesSingle(m_elementsTake.at(0).images, "png");
    if ((m_elementsTake.size() >= 1) && (m_elementsTake.at(0).style == TrenTakePutDefines::esPictureRandom))
        loadPicturesSingle(m_elementsTake.at(0).images, "png"); //"gif");
    ui->lblFullPicture->setVisible((m_elementsTake.size() == 1) &&
                                   (m_elementsTake.at(0).style == TrenTakePutDefines::esPictureSplit));

    if ((m_elementsTake.size() == 1) && (m_elementsTake.at(0).style == TrenTakePutDefines::esPicturePair))
        loadPicturesPair(m_elementsTake.at(0).images);

    m_markerObj = params["marker"].toObject();
    m_backgroundObj = params["background"].toObject();

    auto objTakeOrder = params["take_order"].toObject();
    auto tto = objTakeOrder["mode"].toString();
    if (tto == "enabled_primary")
        m_takeTakeOrder = TrenTakePutDefines::toEnabledPrimary;
    if (tto == "all_by_order")
        m_takeTakeOrder = TrenTakePutDefines::toAllByOrder;
    m_timeFixTake = objTakeOrder["time"].toInt();

    auto arrPutZones = params["put_zones"].toArray();
    auto arrPutElements = params["put_elements"].toArray();
    setZones(arrPutZones, m_zonesPut);
    setElements(arrPutElements, m_elementsPut, TrenTakePutDefines::gsPut);

    auto objPutOrder = params["put_order"].toObject();
    auto pto = objPutOrder["mode"].toString();
    if (pto == "enabled_primary")
        m_putTakeOrder = TrenTakePutDefines::toEnabledPrimary;
    if (pto == "all_by_order")
        m_putTakeOrder = TrenTakePutDefines::toAllByOrder;
    m_timeFixPut = objPutOrder["time"].toInt();

    auto sStage = params["stage"].toString();
    if (sStage == "take_put")
        m_stageMode = TrenTakePutDefines::smTakePut;
    else
    if (sStage == "all_elements")
        m_stageMode = TrenTakePutDefines::smAllElements;
    m_delayAfterStage = params["delay_after_stage"].toInt();

    auto scale = params["scale"].toInt();
    ui->cbScale->setCurrentIndex(scale);

    auto objSS = params["sounds"].toObject();
    m_soundSheme.ok = objSS["ok"].toString();
    m_soundSheme.error = objSS["error"].toString();
    m_soundSheme.scene = objSS["scene"].toString();
    m_soundSheme.onTarget = objSS["on_target"].toString();

    QTimer::singleShot(0, this, &TrenTakePutExecute::start);
}

void TrenTakePutExecute::closeEvent(QCloseEvent *event)
{
    m_isClosed = true;
    hidePatientWindow();

    //! Переехало из деструктора. Подозрение на нерегулярный сбой.
    //! Но для срабатывания необходимо перед delete вызывать close();
    if (m_driver)
    {
        m_driver->stop();
        m_driver->deleteLater();
    }
    QWidget::closeEvent(event);
}

void TrenTakePutExecute::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
//    QSize size = event->size();
    QSize size = ui->gvGame->geometry().size();
    setSceneSize(size);
}

void TrenTakePutExecute::start()
{
    m_driver = static_cast<AAnalyserApplication*>(QApplication::instance())->
            getDriverByFormats(QStringList() << ChannelsDefines::cfDecartCoordinates);
    if (m_driver)
    {
        m_dcControl = dynamic_cast<DeviceProtocols::DecartCoordControl*>(m_driver);

        connect(m_driver, &Driver::sendData, this, &TrenTakePutExecute::getData);
        connect(m_driver, &Driver::communicationError, this, &TrenTakePutExecute::on_communicationError);

        m_kard = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedPatient();
        MetodicDefines::MetodicInfo mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedMetodic();
//        ui->lblProbeTitle->setText(probeParams().name + " - " + m_kard.fio);
        m_trd->newTest(m_kard.uid, mi.uid);

        // По формату получаем список каналов этого формата, которые передает драйвер, заносим их в список для выбора
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
        generateNewScene(false);
    }
    else
    {
        QMessageBox::warning(this, tr("Предупреждение"), tr("Отсутствует необходимое подключение для работы теста"));
        static_cast<ExecuteWidget*>(parent())->showDB();
    }
}

void TrenTakePutExecute::getData(DeviceProtocols::DeviceData *data)
{
    if (ui->cbSelectChannel->currentData(ChannelsUtils::ChannelUidRole).toString() == data->channelId())
    {
        ui->lblCommunicationError->setVisible(false);

        auto* multiData = static_cast<DeviceProtocols::MultiData*>(data);
        if (multiData->subChanCount() > 0)
        {
            QPointF rec = qvariant_cast<QPointF>(multiData->value(0));
            if (m_marker)
            {
                //! Установка маркера
                double mx = rec.x() / (128 / scaleMultiplier()) * (m_scene->sceneRect().width() / 2);
                double my = - rec.y() / (128 / scaleMultiplier()) * (m_scene->sceneRect().height() / 2);

                if (mx - m_marker->boundingRect().width() / 2 < m_scene->sceneRect().x() + m_bndLeft * m_propX)
                    mx = m_scene->sceneRect().x() + m_bndLeft * m_propX + m_marker->boundingRect().width() / 2;
                if (mx > m_scene->sceneRect().x() + m_scene->sceneRect().width() - m_bndRight * m_propX - m_marker->boundingRect().width() / 2)
                    mx = m_scene->sceneRect().x() + m_scene->sceneRect().width() - m_bndRight * m_propX - m_marker->boundingRect().width() / 2;
                if (my - m_marker->boundingRect().height() / 2 < m_scene->sceneRect().y() + m_bndTop * m_propY)
                    my = m_scene->sceneRect().y() + m_bndTop * m_propY + m_marker->boundingRect().height() / 2;
                if (my > m_scene->sceneRect().y() + m_scene->sceneRect().height() - m_bndBottom * m_propY - m_marker->boundingRect().height() / 2)
                    my = m_scene->sceneRect().y() + m_scene->sceneRect().height() - m_bndBottom * m_propY - m_marker->boundingRect().height() / 2;

                m_marker->setPos(mx - m_marker->boundingRect().width() / 2,
                                 my - m_marker->boundingRect().height() / 2);

                ui->wgtAdvChannels->getData(data);

                //! Положение фигуры, захваченной маркером
                if (m_elementTake &&
                        (m_gameStage == TrenTakePutDefines::gsPut ||
                         m_gameStage == TrenTakePutDefines::gsPutProcess))
                {
                    double x = m_marker->pos().x() + m_marker->boundingRect().width() / 2 - m_elementTake->boundingRect().width() / 2;
                    double y = m_marker->pos().y() + m_marker->boundingRect().height() / 2 - m_elementTake->boundingRect().height() / 2;
                    m_elementTake->setPos(x, y);
                }

                //! Взаимодействие элементов
                elementsInteraction();

                m_scene->update(m_scene->sceneRect());
            }
        }
    }
}

void TrenTakePutExecute::on_communicationError(const QString &drvName, const QString &port, const int errorCode)
{
    Q_UNUSED(errorCode);
    ui->lblCommunicationError->setText(QString(tr("Ошибка связи с устройством") + ": %1 (" + tr("порт") + ": %2)").
                                       arg(drvName).arg(port));
    ui->lblCommunicationError->setVisible(true);
}

void TrenTakePutExecute::on_selectChannel(int chanIdx)
{
    qDebug() << chanIdx
             << ui->cbSelectChannel->itemData(chanIdx, ChannelsUtils::ChannelUidRole)
             << ui->cbSelectChannel->itemData(chanIdx, ChannelsUtils::SubChanNumRole);
}

void TrenTakePutExecute::on_zeroing()
{
    auto chanUid = ui->cbSelectChannel->currentData(ChannelsUtils::ChannelUidRole).toString();
    if (m_dcControl && chanUid != "")
        m_dcControl->zeroing(chanUid);
}

void TrenTakePutExecute::on_scaleChange(int scaleIdx)
{
    Q_UNUSED(scaleIdx);
}

void TrenTakePutExecute::on_advChannelsClicked(bool checked)
{
    ui->wgtAdvChannels->setVisible(checked);
}

void TrenTakePutExecute::setSceneSize(QSize &size)
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

void TrenTakePutExecute::setZones(const QJsonArray &arrZones, QList<TrenTakePutDefines::GameZoneInfo> &zones)
{
    zones.clear();
    for (int i = 0; i < arrZones.size(); ++ i)
    {
        auto obj = arrZones.at(i).toObject();
        TrenTakePutDefines::GameZoneInfo zi;

        auto pos = obj["position"].toString();
        if (pos == "random")
            zi.posKind = TrenTakePutDefines::pkRandom;
        else
        if (pos == "fixed")
            zi.posKind = TrenTakePutDefines::pkFixed;

        zi.x_min = obj["x_min"].toInt();
        zi.x_max = obj["x_max"].toInt();
        zi.y_min = obj["y_min"].toInt();
        zi.y_max = obj["y_max"].toInt();
        zi.x = obj["x"].toInt();
        zi.y = obj["y"].toInt();
        zi.width = obj["width"].toInt();
        zi.height = obj["height"].toInt();
        zi.position = obj["positionCode"].toInt();

        zones << zi;
    }
}

void TrenTakePutExecute::setElements(const QJsonArray &arrElements,
                                     QList<TrenTakePutDefines::GameElementInfo> &elements,
                                     TrenTakePutDefines::GameStage stage)
{
    elements.clear();
    for (int i = 0; i < arrElements.size(); ++i)
    {
        auto obj = arrElements.at(i).toObject();
        TrenTakePutDefines::GameElementInfo ei;

        auto sStyle = obj["style"].toString();
        if (sStyle == "picture_fixed")
            ei.style = TrenTakePutDefines::esPictureFixed;
        else
        if (sStyle == "picture_random")
            ei.style = TrenTakePutDefines::esPictureRandom;
        else
        if (sStyle == "picture_pair")
            ei.style = TrenTakePutDefines::esPicturePair;
        else
        if (sStyle == "picture_split")
            ei.style = TrenTakePutDefines::esPictureSplit;
        else
        if (sStyle == "drawing")
            ei.style = TrenTakePutDefines::esDrawing;

        ei.enabled = obj["enabled"].toBool();
        ei.code = obj["code"].toInt();
        ei.images = obj["images"].toString();
        ei.isRepaint = obj["repaint"].toBool();
        ei.color = BaseUtils::strRGBAToColor(obj["color"].toString());
        ei.movableWithMarker = (stage == TrenTakePutDefines::gsTake);

        auto tl = obj["present_time"].toInt();
        if (tl <= 1000)
        {
            ei.timeLimitMin = tl;
            ei.timeLimitMax = tl;
        }
        else
        {
            ei.timeLimitMin = tl / 1000;
            ei.timeLimitMax = tl % 1000;
        }

        auto drawing = obj["drawing"].toString();
        if (drawing == "rectangle")
            ei.drawing = TrenTakePutDefines::edRectangle;
        else
        if (drawing == "circle")
            ei.drawing = TrenTakePutDefines::edCircle;

        ei.isMobile = obj["mobile"].toBool();
        ei.movingMaxForce = obj["moving_max_force"].toInt();
        ei.movingMaxSpeed = obj["moving_max_speed"].toInt();

        auto ml = obj["moving_law"].toString();
        if (ml == "random_force")
            ei.movingLaw = TrenTakePutDefines::mlRandomForce;
        else
        if (ml == "right_to_left")
            ei.movingLaw = TrenTakePutDefines::mlRightToLeft;
        else
        if (ml == "left_to_right")
            ei.movingLaw = TrenTakePutDefines::mlLeftToRight;
        else
        if (ml == "up_to_down")
            ei.movingLaw = TrenTakePutDefines::mlUpToDown;
        else
        if (ml == "down_to_up")
            ei.movingLaw = TrenTakePutDefines::mlDownToUp;

        auto dp = obj["done_process"].toString();
        if (dp == "hide")
            ei.doneProcess = TrenTakePutDefines::dpHide;
        else
        if (dp == "show")
            ei.doneProcess = TrenTakePutDefines::dpShow;
        else
        if (dp == "bang")
            ei.doneProcess = TrenTakePutDefines::dpBang;

        elements << ei;
    }
}

void TrenTakePutExecute::setMarker(const QJsonObject &objMarker)
{
    auto style = objMarker["style"].toString();
    if (style == "picture")
    {
        QPixmap mpmp(":/images/Games/" + objMarker["image"].toString());
        if (objMarker["repaint"].toBool())
            BaseUtils::setColoredPicture(mpmp, BaseUtils::strRGBAToColor(objMarker["color"].toString()));
        m_marker = new TrenTakePutDefines::MarkerElement(mpmp);

        auto stAdv = objMarker["advanced"].toString();
        if (stAdv == "trace_on_target")
            m_targetAdvMode = TrenTakePutDefines::tamTraceOnTarget;
    }
}

void TrenTakePutExecute::setBackground(const QJsonObject &objBackground)
{

    auto style = objBackground["style"].toString();
    if (style == "picture")
    {
        m_background = new TrenTakePutDefines::BackgroundElement(TrenTakePutDefines::bkgmPicture);
        m_background->assignPixmap(":/images/backgrounds/" + objBackground["image"].toString());
        m_background->setRect(m_scene->sceneRect());
        m_background->setZValue(zlvlBackground);
    }
    else
    if (style == "plate")
    {
        m_background = new TrenTakePutDefines::BackgroundElement(TrenTakePutDefines::bkgmPlate);
        m_background->assignPixmap(":/images/plite_textures/" + objBackground["image"].toString());
        m_background->setRect(m_scene->sceneRect());
        m_background->setZValue(zlvlBackground);
    }
    auto bo = objBackground["borders"].toObject();
    m_bndTop = bo["top"].toInt();
    m_bndBottom = bo["bottom"].toInt();
    m_bndLeft = bo["left"].toInt();
    m_bndRight = bo["right"].toInt();

//    QSize size = ui->gvGame->geometry().size();
//    setSceneSize(size);
}

void TrenTakePutExecute::setChannels()
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

void TrenTakePutExecute::elementsInteraction()
{
    //! Проверка, захватил ли маркер элемент
    auto* element = markerOnGameElement();

    //! На этапах фиксации захвата или фиксации укладки
    if (m_gameStage == TrenTakePutDefines::gsTakeProcess ||
        m_gameStage == TrenTakePutDefines::gsPutProcess)
    {
        if (m_targetAdvMode == TrenTakePutDefines::tamTraceOnTarget)
        {
            m_marker->setShotTrace(element);
            if (m_soundSheme.onTarget != "")
            {
                if (element)
                {
                    if (m_player.state() != QMediaPlayer::PlayingState)
                    {
                        m_player.setMedia(QUrl("qrc:/sound/" + m_soundSheme.onTarget));
                        m_player.play();
                    }
                }
                else
                    m_player.stop();
            }
        }

        //! Элемент потерян
        if (!element)
        {
            if (m_gameStage == TrenTakePutDefines::gsTakeProcess)
            {
                m_gameStage = TrenTakePutDefines::gsTake;
                m_elementTake = nullptr;
            }
            else
            if (m_gameStage == TrenTakePutDefines::gsPutProcess)
            {
                m_gameStage = TrenTakePutDefines::gsPut;
                m_elementPut = nullptr;
            }
        }
        else
        //! Элемент не потерян
        {
            m_pos = element->pos();
            if (m_gameStage == TrenTakePutDefines::gsTakeProcess)
            {
                //! Перешли на другой элемент при захвате - потерять предыдущий
                if (element != m_elementTake)
                {
                    m_gameStage = TrenTakePutDefines::gsTake;
                    m_elementTake = nullptr;
                }
            }
            else
            if (m_gameStage == TrenTakePutDefines::gsPutProcess)
            {
                //! Перешли на другой элемент при укладке - потерять предыдущий
                if (element != m_elementPut)
                {
                    m_gameStage = TrenTakePutDefines::gsPut;
                    m_elementPut = nullptr;
                }
            }
        }

        processStageWorking();
    }
    else
    {
        //! Захватили ли элемент или положили ли элемент в зону
        if (element)
        {
            m_pos = element->pos();
            if (element->elementInfo()->enabled
                    &&
                    ((m_gameStage == TrenTakePutDefines::gsTake) ||
                     ((m_gameStage == TrenTakePutDefines::gsPut) &&
                      (element->code() == m_elementTake->code()))))
            {
                if (m_gameStage == TrenTakePutDefines::gsTake)
                {
                    if (m_timeFixTake == 0)
                    {
                        m_elementTake = element;
                        fixingTake();
                    }
                    else
                    {
                        m_gameStage = TrenTakePutDefines::gsTakeProcess;
                        m_elementTake = element;                        
                        m_fixCount = 0;
                    }
                }
                else
                if (m_gameStage == TrenTakePutDefines::gsPut)
                {
                    if (m_timeFixPut == 0)
                    {
                        m_elementPut = element;
                        fixingStage();
                    }
                    else
                    {
                        m_gameStage = TrenTakePutDefines::gsPutProcess;
                        m_elementPut = element;
                        m_fixCount = 0;
                    }
                }
                m_isError = false;
            }
            else
            {
                if (m_gameStage == TrenTakePutDefines::gsTake)
                    fixingError();
                else
                if (m_gameStage == TrenTakePutDefines::gsPut)
                {
                    if (m_timeFixPut == 0)
                        fixingError();
                    else
                    {
                        m_gameStage = TrenTakePutDefines::gsPutProcess;
                        m_elementPut = element;
                        m_fixCount = 0;
                    }
                }
            }
        }
        else
        {
            //! Игры без этапа укладки - сразу, как перешли к укладке, генерим новую сцену
            if (m_zonesPut.size() == 0 && m_elementsPut.size() == 0 &&
                    m_gameStage == TrenTakePutDefines::gsPut)
                fixingStage();
            m_isError = false;
        }
    }

    elementsTimeLimitWorking();
    elementsMobileWorking();
}

void TrenTakePutExecute::processStageWorking()
{
    ++m_fixCount;
    int timeTake = m_timeFixTake;
    if (m_gameStage == TrenTakePutDefines::gsPutProcess)
        timeTake = m_timeFixPut;
    if (m_fixCount >= timeTake * m_frequency)
    {
        if (m_gameStage == TrenTakePutDefines::gsTakeProcess)
        {
            fixingTake();
        }
        else
        if (m_gameStage == TrenTakePutDefines::gsPutProcess)
        {
            int takeCode = -1;
            if (m_elementTake)
                takeCode = m_elementTake->code();
            int putCode = -1;
            if (m_elementPut)
                putCode = m_elementPut->code();

            if (takeCode == putCode)
                fixingStage();
            else
                fixingError();
        }
    }
}

void TrenTakePutExecute::elementsTimeLimitWorking()
{
    bool allIsProcessed = true;
    auto items = m_scene->items();
    for (int i = 0; i < items.size(); ++i)
    {
        auto* item = items[i];
        if (item != m_marker && item != m_background)
        {
            auto* ge = static_cast<TrenTakePutDefines::GameElement*>(item);
            if (ge->presentTime() > 0)
            {
                ge->incTimeCounter(1.0 / m_frequency);
                if (!ge->isProcessed())
                {
                    if (ge->timeCounter() >= ge->presentTime())
                    {
                        ge->setProcessed(true);
                        ge->setVisible(false);
                        m_elementTake = ge;
                        ++m_putElementCount;
                        fixingError();
                    }
                }
                if (!ge->isProcessed())
                    allIsProcessed = false;
            }
            else
                allIsProcessed = false;
        }
    }

    //! Все погашены, - тогда новый этап
    if (allIsProcessed && m_elementTake)
        fixingStage();
}

void TrenTakePutExecute::elementsMobileWorking()
{
    auto items = m_scene->items();
    for (int i = 0; i < items.size(); ++i)
    {
        auto* item = items[i];
        if (item != m_marker && item != m_background)
        {
            auto* ge = static_cast<TrenTakePutDefines::GameElement*>(item);
            if (!ge->isProcessed() && ge->elementInfo()->isMobile)
            {
                if (ge->elementInfo()->movingLaw == TrenTakePutDefines::mlRandomForce)
                    setRandomWorkMobilePosition(ge);
                else
                if (ge->elementInfo()->movingLaw == TrenTakePutDefines::mlLeftToRight ||
                    ge->elementInfo()->movingLaw == TrenTakePutDefines::mlRightToLeft ||
                    ge->elementInfo()->movingLaw == TrenTakePutDefines::mlUpToDown ||
                    ge->elementInfo()->movingLaw == TrenTakePutDefines::mlDownToUp)
                {
                    setLinearMovingMobilePosition(ge);
                    if (!m_scene->sceneRect().contains(ge->pos()))
                    {
                        ge->setProcessed(true);
                        ge->setVisible(false);
                        m_elementTake = ge;
                        ++m_putElementCount;
                        fixingError();
                        fixingStage();
                        return;
                    }
                }
            }
        }
    }
}

void TrenTakePutExecute::setRandomWorkMobilePosition(TrenTakePutDefines::GameElement *ge)
{
    auto randomForce = [&](const double v, const double min, const double max) -> double
    {
        if (v > min / 2 && v < max / 2)
            return qrand() % ge->elementInfo()->movingMaxForce * 2 - ge->elementInfo()->movingMaxForce;
        else
        if (v < min / 2)
            return qrand() % ge->elementInfo()->movingMaxForce * 2;
        else
            return - qrand() % ge->elementInfo()->movingMaxForce * 2;
    };

    double f = randomForce(ge->pos().x(), m_scene->sceneRect().x(), m_scene->sceneRect().right());
    double vx = f / 50 + ge->vx();
    double x = ge->pos().x() + vx;
    f = randomForce(ge->pos().y(), m_scene->sceneRect().y(), m_scene->sceneRect().bottom());
    double vy = f / 50 + ge->vy();
    double y = ge->pos().y() + vy;
    if (fabs(vx) > 5)
        vx = 0;
    if (fabs(vy) > 5)
        vy = 0;
    ge->setSpeed(vx, vy);

    ge->setPos(x, y);
}

void TrenTakePutExecute::setLinearMovingMobilePosition(TrenTakePutDefines::GameElement *ge)
{
    double vx = ge->vx();
    double vy = ge->vy();
    double x = ge->pos().x();
    double y = ge->pos().y();

    int maxSpeed = 10;
    if (ge->elementInfo()->movingMaxSpeed > 0)
        maxSpeed = ge->elementInfo()->movingMaxSpeed;
    if (ge->elementInfo()->movingLaw == TrenTakePutDefines::mlLeftToRight)
    {
        if (vx == 0)
            vx = qrand() % maxSpeed;
        x = x + vx;
    }
    else
    if (ge->elementInfo()->movingLaw == TrenTakePutDefines::mlRightToLeft)
    {
        if (vx == 0)
            vx = qrand() % maxSpeed;
        x = x - vx;
    }
    else
    if (ge->elementInfo()->movingLaw == TrenTakePutDefines::mlUpToDown)
    {
        if (vy == 0)
            vy = qrand() % maxSpeed;
        y = y + vy;
    }
    else
    if (ge->elementInfo()->movingLaw == TrenTakePutDefines::mlDownToUp)
    {
        if (vy == 0)
            vy = qrand() % maxSpeed;
        y = y - vy;
    }
    ge->setPos(x, y);
    ge->setSpeed(vx, vy);
}

void TrenTakePutExecute::fixingTake()
{
    m_elementTake->setZValue(zlvlTakeElements);
    m_gameStage = TrenTakePutDefines::gsPut;
    if (m_soundSheme.ok != "")
    {
        m_player.setMedia(QUrl("qrc:/sound/" + m_soundSheme.ok));
        m_player.play();
    }
}

void TrenTakePutExecute::fixingStage()
{
    //! Нарисовать взрыв
    if (m_elementTake->elementInfo()->doneProcess == TrenTakePutDefines::dpBang)
    {
        auto *ei = m_elementTake->elementInfo();
        QPixmap pm(":/images/Games/bang.png");
        m_elementTake->assignElementInfo(ei, &pm);
    }

    m_gameStage = TrenTakePutDefines::gsTake;
    m_elementTake->setProcessed(true);
    if (m_elementTake->elementInfo()->style != TrenTakePutDefines::esPictureSplit)
    {
        if (m_elementTake->elementInfo()->doneProcess == TrenTakePutDefines::dpHide)
            m_elementTake->setVisible(false);
    }
    else
    {
        m_elementTake->setZValue(zlvlElements);
        m_elementTake->setPos(m_pos); //! Принудительное позиционирование элемента по позиции зоны укладки
    }
    m_elementTake = nullptr;
    if (m_stageMode == TrenTakePutDefines::smTakePut)
        generateNewScene(true);
    else
    if (m_stageMode == TrenTakePutDefines::smAllElements)
    {
        if (m_putElementCount >= m_zonesTake.size() - 1)
        {
            //! Задержка, чтобы зафиксировать собранную сцену перед генерацией новой
            delayScene();
            generateNewScene(true);
        }
        else
        {
            ++m_putElementCount;
            if (m_soundSheme.ok != "")
            {
                m_player.setMedia(QUrl("qrc:/sound/" + m_soundSheme.ok));
                m_player.play();
            }
        }
    }
}

void TrenTakePutExecute::fixingError()
{
    if (!m_isError)
    {
        ++m_errorsCount;
        if (m_score > 0)
            --m_score;
        showFactors();
        m_isError = true;
        if (m_soundSheme.error != "")
        {
            m_player.setMedia(QUrl("qrc:/sound/" + m_soundSheme.error));
            m_player.play();
        }
    }
}

void TrenTakePutExecute::delayScene()
{
    if (m_delayAfterStage > 0)
    {
        m_scene->update(m_scene->sceneRect());
        QTime time;
        time.start();
        while (time.elapsed() < m_delayAfterStage)
        {
            QApplication::processEvents();
        }
    }
}

void TrenTakePutExecute::generateNewScene(const bool isAddScore)
{
    if (!m_isClosed)
    {
        m_scene->clear();
        for (int i = 0; i < m_zonesTake.size(); ++i)
            m_zonesTake[i].clearElement();
        for (int i = 0; i < m_zonesPut.size(); ++i)
            m_zonesPut[i].clearElement();
        m_elementTake = nullptr;
        m_elementPut = nullptr;

        setBackground(m_backgroundObj);

        if (m_elementsTake.size() > 0 && m_elementsPut.size() > 0 &&
            m_zonesTake.size() > 0 && m_zonesPut.size() > 0)
        {
            //! Распределение по отдельным позициям
            if (m_elementsTake.at(0).style == TrenTakePutDefines::esPictureFixed ||
                m_elementsTake.at(0).style == TrenTakePutDefines::esDrawing)
            {
                allocBySeparatePositions(m_takeTakeOrder, m_zonesTake, m_elementsTake, zlvlElements);
                allocBySeparatePositions(m_putTakeOrder, m_zonesPut, m_elementsPut, zlvlZones);
            }
            else
            //! Распределение парных
            if (m_elementsTake.at(0).style == TrenTakePutDefines::esPicturePair)
            {
                allocPairPictires();
            }
            else
            //! Распределение разделенных
            if (m_elementsTake.at(0).style == TrenTakePutDefines::esPictureSplit)
            {
                allocSplitPictures();
            }
        }
        else
        if (m_elementsTake.size() > 0 && m_elementsPut.size() == 0 &&
            m_zonesTake.size() > 0 && m_zonesPut.size() == 0)
        {
            //! Распределение по отдельным позициям
            if (m_elementsTake.at(0).style == TrenTakePutDefines::esPictureRandom)
            {
                allocByRandomPositions(m_zonesTake, m_elementsTake);
            }
        }

        m_scene->addItem(m_background);

        setMarker(m_markerObj);
        m_scene->addItem(m_marker);
        m_marker->setZValue(zlvlMarker);
        m_marker->setPos(0 - m_marker->boundingRect().width() / 2,
                         0 - m_marker->boundingRect().height() / 2);
        m_putElementCount = 0;

        if (isAddScore)
        {
            m_score += (m_zonesTake.size() * 2);
            showFactors();
            if (m_soundSheme.scene != "")
            {
                m_player.setMedia(QUrl("qrc:/sound/" + m_soundSheme.scene));
                m_player.play();
            }
        }
    }
}

void TrenTakePutExecute::allocPairPictires()
{
    if (m_elementsTake.size() != 1 || m_elementsPut.size() != 1 ||
            m_zonesTake.size() != m_zonesPut.size())
        return;

    for (int i = 0; i < m_zonesTake.size(); ++i)
    {
        int picNum = getNextPictureNumber(m_filesPair.size());
        QPixmap pixTake(m_filesPair.at(picNum).take);
        QPixmap pixPut(m_filesPair.at(picNum).put);

        auto* elementTake = allocElement(m_zonesTake, &m_elementsTake[0], &pixTake, zlvlElements);
        auto* elementPut = allocElement(m_zonesPut, &m_elementsPut[0], &pixPut, zlvlZones);
        elementTake->setCode(i+1);
        elementPut->setCode(i+1);
    }
}

void TrenTakePutExecute::allocSplitPictures()
{
    if (m_elementsTake.size() != 1 || m_elementsPut.size() != 1)
        return;

    int picNum = getNextPictureNumber(m_filesSingle.size());

    QPixmap pixAll(m_elementsTake.at(0).images + m_filesSingle.at(picNum));
    ui->lblFullPicture->setPixmap(pixAll.scaled(ui->frControl->geometry().width(), ui->frControl->geometry().width()));
    if (m_patientWindow)
        m_patientWindow->setSamplePixmap(pixAll);

    //! Масштабирование
    pixAll = pixAll.scaled(m_zonesTake.at(0).width * 2 * m_prop,
                           m_zonesTake.at(0).height * 2 * m_prop,
                           Qt::KeepAspectRatio);

    if (m_zonesTake.size() == 4 && m_zonesPut.size() == 4) // || m_zonesTake.size() == 9)
    {
        auto pixLT = pixAll.copy(0, 0, pixAll.width() / 2, pixAll.height() / 2);
        auto pixRT = pixAll.copy(pixAll.width() / 2, 0, pixAll.width() / 2, pixAll.height() / 2);
        auto pixLD = pixAll.copy(0, pixAll.height() / 2, pixAll.width() / 2, pixAll.height() / 2);
        auto pixRD = pixAll.copy(pixAll.width() / 2, pixAll.height() / 2, pixAll.width() / 2, pixAll.height() / 2);

        auto* takeLT = allocElement(m_zonesTake, &m_elementsTake[0], &pixLT, zlvlElements);
        auto* takeRT = allocElement(m_zonesTake, &m_elementsTake[0], &pixRT, zlvlElements);
        auto* takeLD = allocElement(m_zonesTake, &m_elementsTake[0], &pixLD, zlvlElements);
        auto* takeRD = allocElement(m_zonesTake, &m_elementsTake[0], &pixRD, zlvlElements);

//        takeElements.clear();
//        takeElements << takeLT << takeRT << takeLD << takeRD;

        auto* putLT = allocElement(m_zonesPut, &m_elementsPut[0], nullptr, zlvlZones, 0);
        auto* putRT = allocElement(m_zonesPut, &m_elementsPut[0], nullptr, zlvlZones, 1);
        auto* putLD = allocElement(m_zonesPut, &m_elementsPut[0], nullptr, zlvlZones, 2);
        auto* putRD = allocElement(m_zonesPut, &m_elementsPut[0], nullptr, zlvlZones, 3);

//        putElements.clear();
//        putElements << putLT << putRT << putLD << putRD;

        auto assignCode = [&](TrenTakePutDefines::GameElement* elementTake,
                              TrenTakePutDefines::GameElement* elementPut)
        {
            auto posPutCode = m_zonesPut.at(elementPut->zoneIdx()).position;
            elementPut->setCode(posPutCode);
            elementTake->setCode(posPutCode);
        };

        assignCode(takeLT, putLT);
        assignCode(takeRT, putRT);
        assignCode(takeLD, putLD);
        assignCode(takeRD, putRD);
    }
}

int TrenTakePutExecute::getNextPictureNumber(const int filesCount)
{
    int retval = -1;
    int cnt = 0;
    do
    {
        retval = qrand() % filesCount;
        ++cnt;
    }
    while (m_filesUsed.contains(retval));

    m_filesUsed.insert(retval);
    if (m_filesUsed.size() == filesCount)
        m_filesUsed.clear();
    return retval;
}

void TrenTakePutExecute::loadPicturesSingle(const QString &folder, const QString &suffix)
{
    m_filesSingle.clear();
    QDir dir = folder;
    QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    foreach (auto fileInfo, list)
        if (fileInfo.suffix() == suffix)
            m_filesSingle << fileInfo.fileName();
}

void TrenTakePutExecute::loadPicturesPair(const QString &folder)
{
    m_filesPair.clear();
    int i = 1;
    QString fnTake = "";
    QString fnPut = "";
    do
    {
        fnTake = folder + "take" + QString::number(i).rightJustified(2, '0') + ".png";
        fnPut = folder + "put" + QString::number(i).rightJustified(2, '0') + ".png";

        if (QFile::exists(fnTake) && QFile::exists(fnPut))
        {
            FilesPair rec(fnTake, fnPut);
            m_filesPair << rec;
        }
        ++i;
    }
    while(QFile::exists(fnTake) && QFile::exists(fnPut));
}

void TrenTakePutExecute::allocByRandomPositions(QList<TrenTakePutDefines::GameZoneInfo> &zones,
                                                QList<TrenTakePutDefines::GameElementInfo> &elements)
{
    if (elements.size() != 1)
        return;

    for (int i = 0; i < zones.size(); ++i)
    {
        int picNum = getNextPictureNumber(m_filesSingle.size());
        QPixmap pixmap(m_elementsTake.at(0).images + m_filesSingle.at(picNum));
        allocElement(zones, &elements[0], &pixmap, zlvlElements);
    }
}

void TrenTakePutExecute::allocBySeparatePositions(TrenTakePutDefines::TakeOrder &takeOrder,
                                                  QList<TrenTakePutDefines::GameZoneInfo> &zones,
                                                  QList<TrenTakePutDefines::GameElementInfo> &elements,
                                                  const int zOrder)
{
    //! Сначала допустимые, потом остальные
    if (takeOrder == TrenTakePutDefines::toEnabledPrimary)
    {
        allocElements(zones, elements, 1, zOrder);
        while (isEmptyZonesPresent(zones))
            allocElements(zones, elements, 0, zOrder);
    }
    else
    //! Все по порядку
    if (takeOrder == TrenTakePutDefines::toAllByOrder)
        if (m_elementsTake.size() == m_zonesTake.size())
            allocElements(zones, elements, -1, zOrder);
}

void TrenTakePutExecute::allocElements(QList<TrenTakePutDefines::GameZoneInfo> &zones,
                                       QList<TrenTakePutDefines::GameElementInfo> &elements,
                                       int enabled,
                                       const int zOrder)
{
    for (int i = 0; i < elements.size(); ++i)
        if (enabled == -1 || enabled == elements.at(i).enabled)
            allocElement(zones, &elements[i], nullptr, zOrder);
}

TrenTakePutDefines::GameElement* TrenTakePutExecute::allocElement(QList<TrenTakePutDefines::GameZoneInfo> &zones,
                                                                  TrenTakePutDefines::GameElementInfo *element,
                                                                  const QPixmap *pixmap,
                                                                  const int zOrder,
                                                                  const int zoneIdx)
{
    auto* gameElement = new TrenTakePutDefines::GameElement();
    gameElement->assignElementInfo(element, pixmap);

    int zoneNum = zoneIdx;
    if (zoneIdx == -1)
    {
        do
            zoneNum = qrand() % zones.size();
        while (zones.at(zoneNum).element != nullptr);
    }

    zones[zoneNum].setElement(gameElement);
    gameElement->setZoneIdx(zoneNum);

    gameElement->setSize(QSizeF(zones[zoneNum].width * m_prop, zones[zoneNum].height * m_prop));

    if (zones[zoneNum].posKind == TrenTakePutDefines::pkFixed)
        gameElement->setPos(zones[zoneNum].x * m_prop - gameElement->boundingRect().width() / 2,
                            zones[zoneNum].y * m_prop - gameElement->boundingRect().height() / 2);
    else
    if (zones[zoneNum].posKind == TrenTakePutDefines::pkRandom)
    {
        int x = zones[zoneNum].x_max;
        if (zones[zoneNum].x_max > zones[zoneNum].x_min)
            x = zones[zoneNum].x_min + qrand() % (zones[zoneNum].x_max - zones[zoneNum].x_min);
        int y = zones[zoneNum].y_max;
        if (zones[zoneNum].y_max > zones[zoneNum].y_min)
            y = zones[zoneNum].y_min + qrand() % (zones[zoneNum].y_max - zones[zoneNum].y_min);
        gameElement->setPos(x * m_prop - gameElement->boundingRect().width() / 2,
                            y * m_prop - gameElement->boundingRect().height() / 2);
    }

    gameElement->setZValue(zOrder);
    m_scene->addItem(gameElement);
    return gameElement;
}

bool TrenTakePutExecute::isEmptyZonesPresent(QList<TrenTakePutDefines::GameZoneInfo> &zones) const
{
    foreach (auto zone, zones)
        if (zone.element == nullptr)
            return true;
    return false;
}

int TrenTakePutExecute::scaleMultiplier() const
{
    int idx = ui->cbScale->currentIndex();
    int retval = 1;
    for (int i = 0; i < idx; ++i)
        retval *= 2;
    return retval;
}

TrenTakePutDefines::GameElement *TrenTakePutExecute::markerOnGameElement()
{
    double mx = m_marker->x() + m_marker->boundingRect().width() / 2;
    double my = m_marker->y() + m_marker->boundingRect().height() / 2;

    auto items = m_scene->items();
    for (int i = 0; i < items.size(); ++i)
    {
        auto* item = items[i];
        if (item != m_marker && item != m_background)
        {
            auto* ge = static_cast<TrenTakePutDefines::GameElement*>(item);
            if (!ge->isProcessed())
                if ((((m_gameStage == TrenTakePutDefines::gsTake) || (m_gameStage == TrenTakePutDefines::gsTakeProcess))
                     && (ge->elementInfo()->movableWithMarker))
                        ||
                    (((m_gameStage == TrenTakePutDefines::gsPut) || (m_gameStage == TrenTakePutDefines::gsPutProcess))
                     && (!ge->elementInfo()->movableWithMarker)))
                {
                    if (mx >= item->x() && mx <= item->x() + item->boundingRect().width() &&
                        my >= item->y() && my <= item->y() + item->boundingRect().height())
                    {
                        return ge;
                    }
                }
        }
    }
    return nullptr;
}

void TrenTakePutExecute::showPatientWindow()
{
    auto winPresent = SettingsProvider::valueFromRegAppCopy("", "PatientWindow", static_cast<QVariant>(true)).toBool();

    if (winPresent && QApplication::desktop()->screenCount() > 1)
    {
        if (!m_patientWindow)
            m_patientWindow = new TrenTakePutPatientWindow(this);
        m_patientWindow->setScene(m_scene);
        m_patientWindow->resize(QApplication::desktop()->availableGeometry(1).size());
        m_patientWindow->move(QApplication::desktop()->availableGeometry(1).x(),
                              QApplication::desktop()->availableGeometry(1).y());
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

void TrenTakePutExecute::hidePatientWindow()
{
    if (m_patientWindow)
    {
        m_patientWindow->hide();
        auto* p = m_patientWindow;
        m_patientWindow = nullptr;
        delete p;
    }
}

void TrenTakePutExecute::showFactors()
{
    QString score = QString(tr("Очки") + " : %1").arg(m_score);
    QString errors = QString(tr("Ошибки") + " : %1").arg(m_errorsCount);
    ui->lblGameScore->setText(score);
    ui->lblGameErrors->setText(errors);
    if (m_patientWindow)
    {
        m_patientWindow->setScore(score);
        m_patientWindow->setErrors(errors);
    }
}
