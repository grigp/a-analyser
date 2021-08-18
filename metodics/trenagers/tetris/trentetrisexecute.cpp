#include "trentetrisexecute.h"
#include "ui_trentetrisexecute.h"

#include <QTimer>
#include <QDesktopWidget>
#include <QMessageBox>

#include "aanalyserapplication.h"
#include "executewidget.h"
#include "baseutils.h"
#include "channelsutils.h"
#include "settingsprovider.h"
#include "trenresultdata.h"
#include "trenresultfactors.h"
#include "tetrisglass.h"

TrenTetrisExecute::TrenTetrisExecute(QWidget *parent) :
    TrenStabExecute (parent),
    ui(new Ui::TrenTetrisExecute)
{
    ui->setupUi(this);
}

TrenTetrisExecute::~TrenTetrisExecute()
{
    delete ui;
}

void TrenTetrisExecute::setParams(const QJsonObject &params)
{
    TrenStabExecute::setParams(params);

    m_movingMode = TrenTetrisDefines::MovingModeValueIndex.value(params["moving_mode"].toString());
    m_complexityMode = TrenTetrisDefines::ComplexityModeValueIndex.value(params["complexity"].toString());
    m_deletingMode = TrenTetrisDefines::DeletingModeValueIndex.value(params["deleting"].toString());

    m_glassHCount = params["width"].toInt();
    m_glassVCount = params["height"].toInt();

    m_cubeImageFileName = params["cube_image"].toString();

    auto objRdm = params["rows_deleting_mode"].toObject();
    m_cubeColor = BaseUtils::strRGBAToColor(objRdm["cube_color"].toString());
    m_glassColor = BaseUtils::strRGBAToColor(objRdm["glass_color"].toString());
    m_lastColor = BaseUtils::strRGBAToColor(objRdm["last_color"].toString());

    auto objCdm = params["colored_deleting_mode"].toObject();
    m_deletingCubeCount = objCdm["count"].toInt();
    m_colorModeColors.clear();
    auto arrCol = objCdm["colors"].toArray();
    for (int i = 0; i < arrCol.size(); ++i)
    {
        auto obj = arrCol.at(i).toObject();
        QColor color = BaseUtils::strRGBAToColor(obj["color"].toString());
        m_colorModeColors << color;
    }

    auto objAmm = params["auto_moving_mode"].toObject();
    m_autoMovingSpeed = objAmm["speed"].toInt();

    m_markerObj = params["marker"].toObject();
    m_glassObj = params["glass"].toObject();
}

void TrenTetrisExecute::on_recording()
{
    TrenStabExecute::on_recording();

    m_rowsDeleted = 0;
    changeRowsDeleted(0);
}

void TrenTetrisExecute::elementsInteraction(DeviceProtocols::DeviceData *data)
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
}


void TrenTetrisExecute::generateNewScene()
{
    TrenStabExecute::generateNewScene();

    setGlass(m_glassObj);
    m_glass->setGeometry(scene()->width(), scene()->height(), m_glassHCount, m_glassVCount);
    scene()->addItem(m_glass);
    m_glass->setZValue(zlvlGlass);
    m_glass->setPos(-scene()->width() / 2, -scene()->height()/ 2);

    setMarker(m_markerObj);
    scene()->addItem(m_marker);
    m_marker->setZValue(zlvlMarker);
    m_marker->setPos(0 - m_marker->boundingRect().width() / 2,
                     0 - m_marker->boundingRect().height() / 2);
}

void TrenTetrisExecute::fillGameParams(QFrame *frame)
{
    TrenStabExecute::fillGameParams(frame);

    QString style = "font-size: 16pt; color: rgb(9,105,156);";
    QString name = tr("Строки");
    m_lblRowsDel = new QLabel(frame);
    m_lblRowsDel->setText(name);
    m_lblRowsDel->setStyleSheet(style);
    frame->layout()->addWidget(m_lblRowsDel);
    pwSetGameParamLabel(name, style);

    changeRowsDeleted(0);
}

void TrenTetrisExecute::setMarker(const QJsonObject &objMarker)
{
    auto style = objMarker["style"].toString();
    if (style == "picture")
    {
        QPixmap mpmp(":/images/Games/" + objMarker["image"].toString());
        if (objMarker["repaint"].toBool())
            BaseUtils::setColoredPicture(mpmp, BaseUtils::strRGBAToColor(objMarker["color"].toString()));
        m_marker = new GraphicCommon::MarkerElement(mpmp);
    }
}

void TrenTetrisExecute::setGlass(const QJsonObject &objGlass)
{
    m_glass = new TetrisGlass();

    auto style = objGlass["style"].toString();
    if (style == "plate")
        m_glass->setStyle(TetrisGlass::stlPlate);
    else
    if (style == "color")
        m_glass->setStyle(TetrisGlass::stlColor);

    m_glass->setPliteImage(objGlass["image"].toString());
    m_glass->setColor(BaseUtils::strRGBAToColor(objGlass["color"].toString()));

    auto objFrames = objGlass["frames"].toObject();
    m_glass->setFrameColor(BaseUtils::strRGBAToColor(objFrames["color"].toString()));
    m_glass->setFrameLeftImage(objFrames["left"].toString());
    m_glass->setFrameRightImage(objFrames["right"].toString());
    m_glass->setFrameBottomImage(objFrames["bottom"].toString());
    m_glass->setFrameCornerLeftImage(objFrames["corner_left"].toString());
    m_glass->setFrameCornerRightImage(objFrames["corner_right"].toString());
    m_glass->setCubeImage(m_cubeImageFileName);
}

void TrenTetrisExecute::changeRowsDeleted(const int value)
{
    m_rowsDeleted += value;
    QString text = tr("Cтроки") + " - " + QString::number(m_rowsDeleted);
    m_lblRowsDel->setText(text);
    pwSetGameParamLabelValue(1, text);
}

