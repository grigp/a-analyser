#include "trentakeputexecute.h"
#include "ui_trentakeputexecute.h"

#include "aanalyserapplication.h"
#include "driver.h"
#include "executewidget.h"
#include "baseutils.h"
#include "channelsutils.h"

#include <QTimer>
#include <QMessageBox>
#include <QPointF>
#include <QDebug>

TrenTakePutExecute::TrenTakePutExecute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrenTakePutExecute)
  , m_scene(new QGraphicsScene(-1000, -1000, 2000, 2000))
{
    ui->setupUi(this);

    ui->lblCommunicationError->setVisible(false);
    QTimer::singleShot(0, this, &TrenTakePutExecute::start);

    ui->cbScale->addItem("1");
    ui->cbScale->addItem("2");
    ui->cbScale->addItem("4");
    ui->cbScale->addItem("8");
    ui->cbScale->addItem("16");
    ui->cbScale->addItem("32");
    ui->cbScale->addItem("64");
    ui->cbScale->addItem("128");

    ui->gvGame->setScene(m_scene);
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

    m_markerObj = params["marker"].toObject();

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

    auto scale = params["scale"].toInt();
    ui->cbScale->setCurrentIndex(scale);
}

void TrenTakePutExecute::closeEvent(QCloseEvent *event)
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

void TrenTakePutExecute::resizeEvent(QResizeEvent *event)
{
    int size = event->size().height();
    if (event->size().width() < event->size().height())
        size = event->size().width();
    m_scene->setSceneRect(-size / 2, - size / 2, size, size);
    m_prop = static_cast<double>(size) / 2000;
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

//        m_kard = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedPatient();
//        MetodicDefines::MetodicInfo mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedMetodic();
//        ui->lblProbeTitle->setText(probeParams().name + " - " + m_kard.fio);
//        m_trd->newTest(m_kard.uid, mi.uid);

//        showPatientWindow(m_params.at(m_probe).stimulCode);

        // По формату получаем список каналов этого формата, которые передает драйвер, заносим их в список для выбора
        setChannels();

        auto chanUid = ui->cbSelectChannel->currentData(ChannelsUtils::ChannelUidRole).toString();
        m_frequency = m_driver->frequency(chanUid);

        m_driver->start();

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
                m_marker->setPos(mx - m_marker->boundingRect().width() / 2,
                                 my - m_marker->boundingRect().height() / 2);

                //! Положение фигуры, захваченной маркером
                if (m_elementTake)
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
        zi.isMobile = obj["mobile"].toBool();
        zi.movingSpeed = obj["moving_speed"].toInt();

        auto ml = obj["moving_law"].toString();
        if (ml == "random_speed")
            zi.movingLaw = TrenTakePutDefines::mlRandomSpeed;

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

        auto drawing = obj["drawing"].toString();
        if (drawing == "rectangle")
            ei.drawing = TrenTakePutDefines::edRectangle;
        else
        if (drawing == "circle")
            ei.drawing = TrenTakePutDefines::edCircle;

        elements << ei;
    }
}

void TrenTakePutExecute::setMarker(const QJsonObject &objMarker)
{
    auto style = objMarker["style"].toString();
    if (style == "picture")
    {
        QPixmap mpmp(":/images/Games/" + objMarker["image"].toString());
        BaseUtils::setColoredPicture(mpmp, BaseUtils::strRGBAToColor(objMarker["color"].toString()));
        m_marker = new QGraphicsPixmapItem(mpmp);
    }
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
}

void TrenTakePutExecute::elementsInteraction()
{
    //! Проверка, захватил ли маркер элемент
    auto* element = markerOnGameElement();

    //! Захватили ли элемент или положили ли элемент в зону
    if (element)
    {
        if (element->elementInfo()->enabled
                &&
                ((m_gameStage == TrenTakePutDefines::gsTake) ||
                 ((m_gameStage == TrenTakePutDefines::gsPut) &&
                  (element->elementInfo()->code == m_elementTake->elementInfo()->code))))
        {
            if (m_gameStage == TrenTakePutDefines::gsTake)
            {
                if (m_timeFixTake == 0)
                {
                    m_gameStage = TrenTakePutDefines::gsPut;
                    m_elementTake = element;
                    m_player.setMedia(QUrl("qrc:/sound/03.wav"));
                    m_player.play();
                }
                else
                {
                    m_gameStage = TrenTakePutDefines::gsTakeProcess;
                    //todo: время процесса и фиксировать выход со сбросом
                }
            }
            else
            if (m_gameStage == TrenTakePutDefines::gsPut)
            {
                if (m_timeFixPut == 0)
                    newStage();
                else
                {
                    m_gameStage = TrenTakePutDefines::gsPutProcess;
                    //todo: время процесса и фиксировать выход со сбросом
                }
            }
            m_isError = false;
        }
        else
        {
            if (!m_isError)
            {
                ++m_errorsCount;
                ui->lblGameErrors->setText(QString(tr("Ошибки") + " : %1").arg(m_errorsCount));
                if (m_score > 0)
                    --m_score;
                ui->lblGameScore->setText(QString(tr("Очки") + " : %1").arg(m_score));
                m_isError = true;
                m_player.setMedia(QUrl("qrc:/sound/04.wav"));
                m_player.play();
            }
        }
    }
    else
        m_isError = false;
}

void TrenTakePutExecute::newStage()
{
    m_gameStage = TrenTakePutDefines::gsTake;
    m_elementTake->setProcessed(true);
    m_elementTake->setVisible(false);
    m_elementTake = nullptr;
    if (m_stageMode == TrenTakePutDefines::smTakePut)
        generateNewScene(true);
    else
    if (m_stageMode == TrenTakePutDefines::smAllElements)
    {
        if (m_putElementCount == m_zonesTake.size() - 1)
            generateNewScene(true);
        else
        {
            ++m_putElementCount;
            m_player.setMedia(QUrl("qrc:/sound/03.wav"));
            m_player.play();
        }
    }
}

void TrenTakePutExecute::generateNewScene(const bool isAddScore)
{
    m_scene->clear();
    for (int i = 0; i < m_zonesTake.size(); ++i)
        m_zonesTake[i].clearElement();
    for (int i = 0; i < m_zonesPut.size(); ++i)
        m_zonesPut[i].clearElement();

    if (m_elementsTake.size() > 0 && m_elementsPut.size() > 0 &&
        m_zonesTake.size() > 0 && m_zonesPut.size() > 0)
    {
        //! Распределение по отдельным позициям
        if (m_elementsTake.at(0).style == TrenTakePutDefines::esPictureFixed ||
            m_elementsTake.at(0).style == TrenTakePutDefines::esDrawing)
        {
            allocBySeparatePositions(m_takeTakeOrder, m_zonesTake, m_elementsTake, 2);
            allocBySeparatePositions(m_putTakeOrder, m_zonesPut, m_elementsPut, 1);
        }
        else
        //! Распределение парных
        if (m_elementsTake.at(0).style == TrenTakePutDefines::esPicturePair)
        {

        }
        else
        //! Распределение разделенных
        if (m_elementsTake.at(0).style == TrenTakePutDefines::esPictureSplit)
        {

        }
    }

    setMarker(m_markerObj);
    m_scene->addItem(m_marker);
    m_marker->setZValue(3);
    m_marker->setPos(0 - m_marker->boundingRect().width() / 2,
                     0 - m_marker->boundingRect().height() / 2);
    m_putElementCount = 0;

    if (isAddScore)
    {
        m_score += (m_zonesTake.size() * 2);
        ui->lblGameScore->setText(QString(tr("Очки") + " : %1").arg(m_score));
        m_player.setMedia(QUrl("qrc:/sound/05.wav"));
        m_player.play();
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
    {
        if (enabled == -1 || enabled == elements.at(i).enabled)
        {
            auto* gameElement = new TrenTakePutDefines::GameElement();
            gameElement->assignElementInfo(&elements[i]);

            int zoneNum = 0;
            do
                zoneNum = qrand() % zones.size();
            while (zones.at(zoneNum).element != nullptr);

            auto zone = zones.at(zoneNum);
            zone.setElement(gameElement);
            zones.replace(zoneNum, zone);


            if (zone.posKind == TrenTakePutDefines::pkFixed)
                gameElement->setPos(zone.x * m_prop - gameElement->boundingRect().width() / 2,
                                    zone.y * m_prop - gameElement->boundingRect().height() / 2);
            else
            if (zone.posKind == TrenTakePutDefines::pkRandom)
            {
                int x = zone.x_min + qrand() % (zone.x_max - zone.x_min);
                int y = zone.y_min + qrand() % (zone.y_max - zone.y_min);
                gameElement->setPos(x * m_prop - gameElement->boundingRect().width() / 2,
                                    y * m_prop - gameElement->boundingRect().height() / 2);
            }

            gameElement->setZValue(zOrder);
            m_scene->addItem(gameElement);


//            int j = 0;
//            qDebug() << elements.at(i).code << m_scene->items().size() << "---------------------";
//            foreach (auto* item, m_scene->items())
//                if (item != m_marker)
//                {
//                    auto* ge = dynamic_cast<TrenTakePutDefines::GameElement*>(item);
//                    qDebug() << j << ge->elementInfo()->code;
//                    ++j;
//                }
        }
    }
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

    foreach (auto* item, m_scene->items())
    {
        if (item != m_marker)
        {
            auto* ge = static_cast<TrenTakePutDefines::GameElement*>(item);
            if (!ge->isProcessed())
                if (((m_gameStage == TrenTakePutDefines::gsTake) && (ge->elementInfo()->movableWithMarker)) ||
                    ((m_gameStage == TrenTakePutDefines::gsPut) && (!ge->elementInfo()->movableWithMarker)))
                {
                    if (mx >= item->x() && mx <= item->x() + item->boundingRect().width() &&
                        my >= item->y() && my <= item->y() + item->boundingRect().height())
                        return ge;
                }
        }
    }
    return nullptr;
}
