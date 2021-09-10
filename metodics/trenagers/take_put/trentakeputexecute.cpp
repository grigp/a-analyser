#include "trentakeputexecute.h"
#include "ui_trentakeputexecute.h"

#include "aanalyserapplication.h"
#include "driver.h"
#include "executewidget.h"
#include "baseutils.h"
#include "channelsutils.h"
#include "trenagerpatientwindow.h"
#include "settingsprovider.h"
#include "testresultdata.h"
#include "trenresultdata.h"
#include "trenresultfactors.h"

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
    TrenStabExecute(parent),
    ui(new Ui::TrenTakePutExecute)
{
    ui->setupUi(this);

    m_filesUsed.clear();
}

TrenTakePutExecute::~TrenTakePutExecute()
{
    delete ui;
}

void TrenTakePutExecute::setParams(const QJsonObject &params)
{
    TrenStabExecute::setParams(params);

    auto arrTakeZones = params["take_zones"].toArray();
    auto arrTakeElements = params["take_elements"].toArray();
    setZones(arrTakeZones, m_zonesTake);
    setElements(arrTakeElements, m_elementsTake, TrenTakePutDefines::gsTake);

    if ((m_elementsTake.size() == 1) && (m_elementsTake.at(0).style == GraphicCommon::esPictureSplit))
        loadPicturesSingle(m_elementsTake.at(0).images, "png");
    if ((m_elementsTake.size() >= 1) && (m_elementsTake.at(0).style == GraphicCommon::esPictureRandom))
        loadPicturesSingle(m_elementsTake.at(0).images, "png"); //"gif");

    if ((m_elementsTake.size() == 1) && (m_elementsTake.at(0).style == GraphicCommon::esPicturePair))
        loadPicturesPair(m_elementsTake.at(0).images);

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
    m_delayAfterStage = params["delay_after_stage"].toInt();

    auto objSS = params["sounds"].toObject();
    m_soundSheme.ok = objSS["ok"].toString();
    m_soundSheme.error = objSS["error"].toString();
    m_soundSheme.scene = objSS["scene"].toString();
    m_soundSheme.onTarget = objSS["on_target"].toString();

    setAdvancedChannelEnable(1, false);
    setAdvancedFunctionTitle(0, tr("для захвата и укладки"));
}

void TrenTakePutExecute::elementsInteraction(DeviceProtocols::DeviceData *data)
{
    TrenStabExecute::elementsInteraction(data);

    auto* multiData = static_cast<DeviceProtocols::MultiData*>(data);
    if (multiData->subChanCount() > 0)
    {
        //! Управление маркером
        QPointF rec = qvariant_cast<QPointF>(multiData->value(0));
        if (m_marker)
        {
            //! Установка маркера
            double mx = rec.x() / (128 / BaseUtils::scaleMultiplier(scale())) * (scene()->sceneRect().width() / 2);
            double my = - rec.y() / (128 / BaseUtils::scaleMultiplier(scale())) * (scene()->sceneRect().height() / 2);

            if (mx - m_marker->boundingRect().width() / 2 < scene()->sceneRect().x() + bndLeft() * propX())
                mx = scene()->sceneRect().x() + bndLeft() * propX() + m_marker->boundingRect().width() / 2;
            if (mx > scene()->sceneRect().x() + scene()->sceneRect().width() - bndRight() * propX() - m_marker->boundingRect().width() / 2)
                mx = scene()->sceneRect().x() + scene()->sceneRect().width() - bndRight() * propX() - m_marker->boundingRect().width() / 2;
            if (my - m_marker->boundingRect().height() / 2 < scene()->sceneRect().y() + bndTop() * propY())
                my = scene()->sceneRect().y() + bndTop() * propY() + m_marker->boundingRect().height() / 2;
            if (my > scene()->sceneRect().y() + scene()->sceneRect().height() - bndBottom() * propY() - m_marker->boundingRect().height() / 2)
                my = scene()->sceneRect().y() + scene()->sceneRect().height() - bndBottom() * propY() - m_marker->boundingRect().height() / 2;

            m_marker->setPos(mx - m_marker->boundingRect().width() / 2,
                             my - m_marker->boundingRect().height() / 2);
        }
    }

    //! Положение фигуры, захваченной маркером
    if (m_elementTake &&
            (m_gameStage == TrenTakePutDefines::gsPut ||
             m_gameStage == TrenTakePutDefines::gsPutProcess))
    {
        double x = m_marker->pos().x() + m_marker->boundingRect().width() / 2 - m_elementTake->boundingRect().width() / 2;
        double y = m_marker->pos().y() + m_marker->boundingRect().height() / 2 - m_elementTake->boundingRect().height() / 2;
        m_elementTake->setPos(x, y);
    }

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

void TrenTakePutExecute::generateNewScene()
{
    TrenStabExecute::generateNewScene();

    if (!isClosed())
    {
        for (int i = 0; i < m_zonesTake.size(); ++i)
            m_zonesTake[i].clearElement();
        for (int i = 0; i < m_zonesPut.size(); ++i)
            m_zonesPut[i].clearElement();
        m_elementTake = nullptr;
        m_elementPut = nullptr;

        if (m_elementsTake.size() > 0 && m_elementsPut.size() > 0 &&
            m_zonesTake.size() > 0 && m_zonesPut.size() > 0)
        {
            //! Распределение по отдельным позициям
            if (m_elementsTake.at(0).style == GraphicCommon::esPictureFixed ||
                m_elementsTake.at(0).style == GraphicCommon::esDrawing)
            {
                allocBySeparatePositions(m_takeTakeOrder, m_zonesTake, m_elementsTake, zlvlElements);
                allocBySeparatePositions(m_putTakeOrder, m_zonesPut, m_elementsPut, zlvlZones);
            }
            else
            //! Распределение парных
            if (m_elementsTake.at(0).style == GraphicCommon::esPicturePair)
            {
                allocPairPictires();
            }
            else
            //! Распределение разделенных
            if (m_elementsTake.at(0).style == GraphicCommon::esPictureSplit)
            {
                allocSplitPictures();
            }
        }
        else
        if (m_elementsTake.size() > 0 && m_elementsPut.size() == 0 &&
            m_zonesTake.size() > 0 && m_zonesPut.size() == 0)
        {
            //! Распределение по отдельным позициям
            if (m_elementsTake.at(0).style == GraphicCommon::esPictureRandom)
            {
                allocByRandomPositions(m_zonesTake, m_elementsTake);
            }
        }

        setMarker(m_markerObj);
        scene()->addItem(m_marker);
        m_marker->setZValue(zlvlMarker);
        m_marker->setPos(0 - m_marker->boundingRect().width() / 2,
                         0 - m_marker->boundingRect().height() / 2);
        m_putElementCount = 0;
    }
}

void TrenTakePutExecute::addScoreNewScene()
{
    if (!isClosed())
    {
        changeGameScore(m_zonesTake.size() * 2);
        if (m_soundSheme.scene != "")
        {
            m_player.setMedia(QUrl("qrc:/sound/" + m_soundSheme.scene));
            m_player.play();
        }
    }
}

void TrenTakePutExecute::fillGameParams(QFrame *frame)
{
    TrenStabExecute::fillGameParams(frame);

    QString style = "font-size: 16pt; color: rgb(255,0,0);";
    QString name = tr("Ошибки");
    m_lblErrors = new QLabel(frame);
    m_lblErrors->setText(name);
    m_lblErrors->setStyleSheet(style);
    frame->layout()->addWidget(m_lblErrors);
    pwSetGameParamLabel(name, style);

    changeErrors(0);
}

void TrenTakePutExecute::fillGameHints(QFrame *frame)
{
    TrenStabExecute::fillGameHints(frame);

    m_lblPicture = new QLabel(frame);
    m_lblPicture->setText("pic");
    m_lblPicture->setVisible((m_elementsTake.size() == 1) &&
                             (m_elementsTake.at(0).style == GraphicCommon::esPictureSplit));
    frame->layout()->addWidget(m_lblPicture);

    m_lblPicturePW = new QLabel();
    m_lblPicturePW->setText("pic");
    m_lblPicturePW->setVisible((m_elementsTake.size() == 1) &&
                               (m_elementsTake.at(0).style == GraphicCommon::esPictureSplit));
    pwAddHintWidget(m_lblPicturePW);
}

void TrenTakePutExecute::on_recording()
{
    TrenStabExecute::on_recording();

    m_errorsCount = 0;
    changeErrors(0);
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
                                     QList<GraphicCommon::GameElementInfo> &elements,
                                     TrenTakePutDefines::GameStage stage)
{
    elements.clear();
    for (int i = 0; i < arrElements.size(); ++i)
    {
        auto obj = arrElements.at(i).toObject();
        GraphicCommon::GameElementInfo ei;

        auto sStyle = obj["style"].toString();
        if (sStyle == "picture_fixed")
            ei.style = GraphicCommon::esPictureFixed;
        else
        if (sStyle == "picture_random")
            ei.style = GraphicCommon::esPictureRandom;
        else
        if (sStyle == "picture_pair")
            ei.style = GraphicCommon::esPicturePair;
        else
        if (sStyle == "picture_split")
            ei.style = GraphicCommon::esPictureSplit;
        else
        if (sStyle == "drawing")
            ei.style = GraphicCommon::esDrawing;

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
            ei.drawing = GraphicCommon::edRectangle;
        else
        if (drawing == "circle")
            ei.drawing = GraphicCommon::edCircle;

        ei.isMobile = obj["mobile"].toBool();
        ei.movingMaxForce = obj["moving_max_force"].toInt();
        ei.movingMaxSpeed = obj["moving_max_speed"].toInt();

        auto ml = obj["moving_law"].toString();
        if (ml == "random_force")
            ei.movingLaw = GraphicCommon::mlRandomForce;
        else
        if (ml == "right_to_left")
            ei.movingLaw = GraphicCommon::mlRightToLeft;
        else
        if (ml == "left_to_right")
            ei.movingLaw = GraphicCommon::mlLeftToRight;
        else
        if (ml == "up_to_down")
            ei.movingLaw = GraphicCommon::mlUpToDown;
        else
        if (ml == "down_to_up")
            ei.movingLaw = GraphicCommon::mlDownToUp;

        auto dp = obj["done_process"].toString();
        if (dp == "hide")
            ei.doneProcess = GraphicCommon::dpHide;
        else
        if (dp == "show")
            ei.doneProcess = GraphicCommon::dpShow;
        else
        if (dp == "bang")
            ei.doneProcess = GraphicCommon::dpBang;

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
        m_marker = new GraphicCommon::MarkerElement(mpmp);

        auto stAdv = objMarker["advanced"].toString();
        if (stAdv == "trace_on_target")
            m_targetAdvMode = TrenTakePutDefines::tamTraceOnTarget;
    }
}

void TrenTakePutExecute::processStageWorking()
{
    ++m_fixCount;
    int timeTake = m_timeFixTake;
    if (m_gameStage == TrenTakePutDefines::gsPutProcess)
        timeTake = m_timeFixPut;
    if (m_fixCount >= timeTake * frequency())
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
    auto items = scene()->items();
    for (int i = 0; i < items.size(); ++i)
    {
        auto* item = items[i];
        if (item != m_marker && item != background())
        {
            auto* ge = static_cast<GraphicCommon::GameElement*>(item);
            if (ge->presentTime() > 0)
            {
                ge->incTimeCounter(1.0 / frequency());
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
    auto items = scene()->items();
    for (int i = 0; i < items.size(); ++i)
    {
        auto* item = items[i];
        if (item != m_marker && item != background())
        {
            auto* ge = static_cast<GraphicCommon::GameElement*>(item);
            if (!ge->isProcessed() && ge->elementInfo()->isMobile)
            {
                if (ge->elementInfo()->movingLaw == GraphicCommon::mlRandomForce)
                    setRandomWorkMobilePosition(ge);
                else
                if (ge->elementInfo()->movingLaw == GraphicCommon::mlLeftToRight ||
                    ge->elementInfo()->movingLaw == GraphicCommon::mlRightToLeft ||
                    ge->elementInfo()->movingLaw == GraphicCommon::mlUpToDown ||
                    ge->elementInfo()->movingLaw == GraphicCommon::mlDownToUp)
                {
                    setLinearMovingMobilePosition(ge);
                    if (!scene()->sceneRect().contains(ge->pos()))
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

void TrenTakePutExecute::setRandomWorkMobilePosition(GraphicCommon::GameElement *ge)
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

    double f = randomForce(ge->pos().x(), scene()->sceneRect().x(), scene()->sceneRect().right());
    double vx = f / 50 + ge->vx();
    double x = ge->pos().x() + vx;
    f = randomForce(ge->pos().y(), scene()->sceneRect().y(), scene()->sceneRect().bottom());
    double vy = f / 50 + ge->vy();
    double y = ge->pos().y() + vy;
    if (fabs(vx) > 5)
        vx = 0;
    if (fabs(vy) > 5)
        vy = 0;
    ge->setSpeed(vx, vy);

    ge->setPos(x, y);
}

void TrenTakePutExecute::setLinearMovingMobilePosition(GraphicCommon::GameElement *ge)
{
    double vx = ge->vx();
    double vy = ge->vy();
    double x = ge->pos().x();
    double y = ge->pos().y();

    int maxSpeed = 10;
    if (ge->elementInfo()->movingMaxSpeed > 0)
        maxSpeed = ge->elementInfo()->movingMaxSpeed;
    if (ge->elementInfo()->movingLaw == GraphicCommon::mlLeftToRight)
    {
        if (vx == 0)
            vx = qrand() % maxSpeed;
        x = x + vx;
    }
    else
    if (ge->elementInfo()->movingLaw == GraphicCommon::mlRightToLeft)
    {
        if (vx == 0)
            vx = qrand() % maxSpeed;
        x = x - vx;
    }
    else
    if (ge->elementInfo()->movingLaw == GraphicCommon::mlUpToDown)
    {
        if (vy == 0)
            vy = qrand() % maxSpeed;
        y = y + vy;
    }
    else
    if (ge->elementInfo()->movingLaw == GraphicCommon::mlDownToUp)
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
    if (m_elementTake->elementInfo()->doneProcess == GraphicCommon::dpBang)
    {
        auto *ei = m_elementTake->elementInfo();
        QPixmap pm(":/images/Games/bang.png");
        m_elementTake->assignElementInfo(ei, &pm);
    }

    m_gameStage = TrenTakePutDefines::gsTake;
    m_elementTake->setProcessed(true);
    if (m_elementTake->elementInfo()->style != GraphicCommon::esPictureSplit)
    {
        if (m_elementTake->elementInfo()->doneProcess == GraphicCommon::dpHide)
            m_elementTake->setVisible(false);
    }
    else
    {
        m_elementTake->setZValue(zlvlElements);
        m_elementTake->setPos(m_pos); //! Принудительное позиционирование элемента по позиции зоны укладки
    }
    m_elementTake = nullptr;
    if (m_stageMode == TrenTakePutDefines::smTakePut)
    {
        generateNewScene();
        addScoreNewScene();
    }
    else
    if (m_stageMode == TrenTakePutDefines::smAllElements)
    {
        if (m_putElementCount >= m_zonesTake.size() - 1)
        {
            //! Задержка, чтобы зафиксировать собранную сцену перед генерацией новой
            delayScene();
            generateNewScene();
            addScoreNewScene();
        }
        else
        {
            changeGameScore(1);

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
        changeErrors(1);
        changeGameScore(-1);
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
        scene()->update(scene()->sceneRect());
        QTime time;
        time.start();
        while (time.elapsed() < m_delayAfterStage)
        {
            QApplication::processEvents();
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
    m_lblPicture->setPixmap(pixAll.scaled(getFrameControlWidth(), getFrameControlWidth()));
    m_lblPicture->setVisible(true);
    m_lblPicturePW->setPixmap(pixAll.scaled(pwGetFrameParamsWidth(), pwGetFrameParamsWidth()));
    m_lblPicturePW->setVisible(true);

    //! Масштабирование
    pixAll = pixAll.scaled(static_cast<int>(m_zonesTake.at(0).width * 2 * prop()),
                           static_cast<int>(m_zonesTake.at(0).height * 2 * prop()),
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

        auto* putLT = allocElement(m_zonesPut, &m_elementsPut[0], nullptr, zlvlZones, 0);
        auto* putRT = allocElement(m_zonesPut, &m_elementsPut[0], nullptr, zlvlZones, 1);
        auto* putLD = allocElement(m_zonesPut, &m_elementsPut[0], nullptr, zlvlZones, 2);
        auto* putRD = allocElement(m_zonesPut, &m_elementsPut[0], nullptr, zlvlZones, 3);

        auto assignCode = [&](GraphicCommon::GameElement* elementTake,
                              GraphicCommon::GameElement* elementPut)
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
                                                QList<GraphicCommon::GameElementInfo> &elements)
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
                                                  QList<GraphicCommon::GameElementInfo> &elements,
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
                                       QList<GraphicCommon::GameElementInfo> &elements,
                                       int enabled,
                                       const int zOrder)
{
    for (int i = 0; i < elements.size(); ++i)
        if (enabled == -1 || enabled == elements.at(i).enabled)
            allocElement(zones, &elements[i], nullptr, zOrder);
}

GraphicCommon::GameElement* TrenTakePutExecute::allocElement(QList<TrenTakePutDefines::GameZoneInfo> &zones,
                                                                  GraphicCommon::GameElementInfo *element,
                                                                  const QPixmap *pixmap,
                                                                  const int zOrder,
                                                                  const int zoneIdx)
{
    auto* gameElement = new GraphicCommon::GameElement();
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

    gameElement->setSize(QSizeF(zones[zoneNum].width * prop(), zones[zoneNum].height * prop()));

    if (zones[zoneNum].posKind == TrenTakePutDefines::pkFixed)
        gameElement->setPos(zones[zoneNum].x * prop() - gameElement->boundingRect().width() / 2,
                            zones[zoneNum].y * prop() - gameElement->boundingRect().height() / 2);
    else
    if (zones[zoneNum].posKind == TrenTakePutDefines::pkRandom)
    {
        int x = zones[zoneNum].x_max;
        if (zones[zoneNum].x_max > zones[zoneNum].x_min)
            x = zones[zoneNum].x_min + qrand() % (zones[zoneNum].x_max - zones[zoneNum].x_min);
        int y = zones[zoneNum].y_max;
        if (zones[zoneNum].y_max > zones[zoneNum].y_min)
            y = zones[zoneNum].y_min + qrand() % (zones[zoneNum].y_max - zones[zoneNum].y_min);
        gameElement->setPos(x * prop() - gameElement->boundingRect().width() / 2,
                            y * prop() - gameElement->boundingRect().height() / 2);
    }

    gameElement->setZValue(zOrder);
    scene()->addItem(gameElement);
    return gameElement;
}

bool TrenTakePutExecute::isEmptyZonesPresent(QList<TrenTakePutDefines::GameZoneInfo> &zones) const
{
    foreach (auto zone, zones)
        if (zone.element == nullptr)
            return true;
    return false;
}

GraphicCommon::GameElement *TrenTakePutExecute::markerOnGameElement()
{
    double mx = m_marker->x() + m_marker->boundingRect().width() / 2;
    double my = m_marker->y() + m_marker->boundingRect().height() / 2;

    auto items = scene()->items();
    for (int i = 0; i < items.size(); ++i)
    {
        auto* item = items[i];
        if (item != m_marker && item != background())
        {
            auto* ge = static_cast<GraphicCommon::GameElement*>(item);
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

void TrenTakePutExecute::finishTest()
{
    //! Добавляем значение специфического показателя для подкласса TrenTakePutExecute: количество ошибок
    addFactorValue(TrenResultFactorsDefines::FaultsUid, m_errorsCount);

    TrenStabExecute::finishTest();
}

void TrenTakePutExecute::changeErrors(const int value)
{
    m_errorsCount += value;
    QString text = tr("Ошибки") + " - " + QString::number(m_errorsCount);
    if (m_lblErrors)
    {
        m_lblErrors->setText(text);
        pwSetGameParamLabelValue(1, text);
    }
}

