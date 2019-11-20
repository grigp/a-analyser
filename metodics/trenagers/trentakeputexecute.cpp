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
    setElements(arrTakeElements, m_elementsTake);

    auto objTakeOrder = params["take_order"].toObject();
    auto tto = objTakeOrder["mode"].toString();
    if (tto == "enabled_primary")
        m_takeTakeOrder = TrenTakePutDefines::toEnabledPrimary;
    if (tto == "all_by_order")
        m_takeTakeOrder = TrenTakePutDefines::toAllByOrder;

    auto arrPutZones = params["put_zones"].toArray();
    auto arrPutElements = params["put_elements"].toArray();
    setZones(arrPutZones, m_zonesPut);
    setElements(arrPutElements, m_elementsPut);

    auto objPutOrder = params["put_order"].toObject();
    auto pto = objPutOrder["mode"].toString();
    if (pto == "enabled_primary")
        m_putTakeOrder = TrenTakePutDefines::toEnabledPrimary;
    if (pto == "all_by_order")
        m_putTakeOrder = TrenTakePutDefines::toAllByOrder;

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

        newScene();
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

//        auto* multiData = static_cast<DeviceProtocols::MultiData*>(data);
//        if (multiData->subChanCount() > 0)
//        {
//            QPointF rec = qvariant_cast<QPointF>(multiData->value(0));
//            qDebug() << rec.x() << rec.y();
//        }
//        else
//            qDebug() << multiData->subChanCount();

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

void TrenTakePutExecute::setElements(const QJsonArray &arrElements, QList<TrenTakePutDefines::GameElementInfo> &elements)
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

        auto drawing = obj["drawing"].toString();
        if (drawing == "rectangle")
            ei.drawing = TrenTakePutDefines::edRectangle;
        else
        if (drawing == "circle")
            ei.drawing = TrenTakePutDefines::edCircle;

        elements << ei;
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

void TrenTakePutExecute::newScene()
{
    m_scene->clear();

    if (m_elementsTake.size() > 0 && m_elementsPut.size() > 0 &&
        m_zonesTake.size() > 0 && m_zonesPut.size() > 0)
    {
        //! Распределение по отдельным позициям
        if (m_elementsTake.at(0).style == TrenTakePutDefines::esPictureFixed ||
            m_elementsTake.at(0).style == TrenTakePutDefines::esDrawing)
        {
            allocBySeparatePositions(m_takeTakeOrder, m_zonesTake, m_elementsTake);
            allocBySeparatePositions(m_putTakeOrder, m_zonesPut, m_elementsPut);
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
}

void TrenTakePutExecute::allocBySeparatePositions(TrenTakePutDefines::TakeOrder &takeOrder,
                                                  QList<TrenTakePutDefines::GameZoneInfo> &zones,
                                                  QList<TrenTakePutDefines::GameElementInfo> &elements)
{
    //! Сначала допустимые, потом остальные
    if (takeOrder == TrenTakePutDefines::toEnabledPrimary)
    {
        allocElements(zones, elements, 1);
        while (isEmptyZonesPresent(zones))
            allocElements(zones, elements, 0);
    }
    else
    //! Все по порядку
    if (takeOrder == TrenTakePutDefines::toAllByOrder)
        if (m_elementsTake.size() == m_zonesTake.size())
            allocElements(zones, elements, -1);
}

void TrenTakePutExecute::allocElements(QList<TrenTakePutDefines::GameZoneInfo> &zones,
                                       QList<TrenTakePutDefines::GameElementInfo> &elements,
                                       int enabled)
{
    foreach (auto element, elements)
    {
        if (enabled == -1 || enabled == element.enabled)
        {
            auto* gameElement = new TrenTakePutDefines::GameElement();
            gameElement->assignElementInfo(&element);

            int zoneNum = 0;
            do
                zoneNum = qrand() % zones.size();
            while (zones.at(zoneNum).element != nullptr);

            auto zone = zones.at(zoneNum);
            zone.setElement(gameElement);
            zones.replace(zoneNum, zone);

            m_scene->addItem(gameElement);
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
