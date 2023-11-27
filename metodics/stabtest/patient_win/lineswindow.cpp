#include "lineswindow.h"
#include "ui_lineswindow.h"

#include <QVariant>
#include <QJsonObject>
#include <QDebug>

#include "baseutils.h"
#include "videoirritant.h"
#include "linesirriant.h"
#include "frontcommentitem.h"

LinesWindow::LinesWindow(QWidget *parent) :
    PatientWindow(parent),
    ui(new Ui::LinesWindow)
  , m_scene(new QGraphicsScene(-1000, -1000, 2000, 2000))
{
    ui->setupUi(this);
    initLines();
}

LinesWindow::~LinesWindow()
{
    delete ui;
}

void LinesWindow::setParams(const QJsonObject &params)
{
    if (m_videoIrritant)
    {
        auto lines = static_cast<LinesIrriant*>(m_videoIrritant->irriant(0));
        lines->setDirection(static_cast<BaseDefines::Directions>(params["direction"].toInt(BaseDefines::dirRight)));

        lines->setWidth(params["width"].toInt(120));
        lines->setSpeed(params["speed"].toInt(200));
        lines->setDutyCycle(params["duty_cycle"].toInt(1));
        lines->setColor(BaseUtils::strRGBAToColor(params["color"].toString("00000000")));
    }
}

void LinesWindow::setDiap(const int diap)
{
    Q_UNUSED(diap);
}

void LinesWindow::setMarker(const double x, const double y)
{
    Q_UNUSED(x);
    Q_UNUSED(y);
    m_scene->update(m_scene->sceneRect());
}

void LinesWindow::run()
{
}

void LinesWindow::stop()
{

}

void LinesWindow::setFrontComment(const QString &comment)
{
    m_frontComment->setText(comment);
}

QVariant LinesWindow::result()
{
    return QVariant();
}

void LinesWindow::initLines()
{
    m_scene->clear();

    m_videoIrritant = new VideoIrritant(m_scene->sceneRect());
    m_videoIrritant->setZValue(100);
    m_scene->addItem(m_videoIrritant);
    m_videoIrritant->setCurrentIrriant(0);
    m_videoIrritant->setTransparent(255);

    m_frontComment = new FrontCommentItem(m_scene->sceneRect());
    m_frontComment->setZValue(101);
    m_frontComment->setText("");
    m_scene->addItem(m_frontComment);

    ui->gvLines->setScene(m_scene);
}
