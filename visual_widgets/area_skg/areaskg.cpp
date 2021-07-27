#include "areaskg.h"
#include "ui_areaskg.h"

#include "gridskg.h"
#include "traceskg.h"
#include "lineskg.h"
#include "signalaccess.h"
#include "gridskgdefines.h"

#include <QDebug>

AreaSKG::AreaSKG(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AreaSKG)
  , m_sceneSKG(new QGraphicsScene(QRectF(-128, -128, 256, 256)))
  , m_gridSKG(new GridSKG(128))
  , m_traceSKG(new TraceSKG(128))
  , m_lineSKG(new LineSKG(128))
{
    ui->setupUi(this);
    m_targets.clear();
    setAreaSKG();
}

AreaSKG::~AreaSKG()
{
    delete ui;
}

int AreaSKG::diap() const
{
    return m_diap;
}

void AreaSKG::setDiap(const int diap)
{
    m_gridSKG->setDiap(diap);
    m_traceSKG->setDiap(diap);
    m_lineSKG->setDiap(diap);
    m_diap = diap;
    ui->panSKG->ensureVisible(QRectF(-m_diap, -m_diap, m_diap * 2, m_diap * 2));
}

void AreaSKG::addMarker()
{
    m_marker = m_sceneSKG->addRect(QRectF(0, 0, 10, 10), QPen(Qt::darkRed), QBrush(Qt::red));
}

void AreaSKG::setMarker(const double x, const double y)
{
    if (m_marker)
    {
        int minS = qMin(ui->panSKG->width(), ui->panSKG->height());
        double prop = static_cast<double>(minS / 2 - GridSKGDefines::I_LABEL_SPACE) / static_cast<double>(m_diap);

        m_marker->setPos(x * prop - m_marker->boundingRect().width() / 2,
                         - y * prop - m_marker->boundingRect().height() / 2);

        if (m_isShowTrace)
            m_traceSKG->add(x, y);
    }
}

void AreaSKG::setMarkerColor(const QColor colorBackground, const QColor colorBorder)
{
    if (m_marker)
    {
        m_marker->setBrush(colorBackground);
        m_marker->setPen(colorBorder);
    }
}

void AreaSKG::showTrace(const bool trace)
{
    m_isShowTrace = trace;
    if (!m_isShowTrace)
        m_traceSKG->clear();
}

void AreaSKG::setSignal(SignalAccess *signal)
{
    m_lineSKG->setSignal(signal);
}

void AreaSKG::setVisibleMarker(const bool visibleMarker)
{
    m_isVisibleMarker = visibleMarker;
    if (m_isVisibleMarker)
        if (!m_marker)
            m_marker = m_sceneSKG->addRect(QRectF(0, 0, 10, 10), QPen(Qt::darkRed), QBrush(Qt::red));
        else
            return;
    else
    {
        if (m_marker)
        {
            m_sceneSKG->removeItem(m_marker);
            delete m_marker;
            m_marker = nullptr;
        }
    }
}

void AreaSKG::setZeroing(const bool zeroing)
{
    if (m_lineSKG)
        m_lineSKG->setZeroing(zeroing);
}

void AreaSKG::setEllipse(const double sizeA, const double sizeB, const double angle)
{
    if (m_lineSKG)
        m_lineSKG->setEllipse(sizeA, sizeB, angle);
}

void AreaSKG::setColorSKG(const QColor &color)
{
    if (m_lineSKG)
        m_lineSKG->setColorSKG(color);
    if (m_traceSKG)
        m_traceSKG->setColorSKG(color);
}

void AreaSKG::setColorEllipse(const QColor &color)
{
    if (m_lineSKG)
        m_lineSKG->setColorEllipse(color);
}

void AreaSKG::addTarget(const double x, const double y, const QColor colorBackground, const QColor colorBorder)
{
    QGraphicsRectItem* target = m_sceneSKG->addRect(QRectF(0, 0, 10, 10), colorBorder, colorBackground);
    m_targets.append(target);

    setTarget(x, y, m_targets.size() - 1);
}

void AreaSKG::setTarget(const double x, const double y, const int idx)
{
    int minS = qMin(ui->panSKG->width(), ui->panSKG->height());
    double prop = static_cast<double>(minS / 2 - GridSKGDefines::I_LABEL_SPACE) / static_cast<double>(m_diap);
    m_targets.at(idx)->setPos(x * prop - m_targets.at(idx)->boundingRect().width() / 2,
                              - y * prop - m_targets.at(idx)->boundingRect().height() / 2);
}

void AreaSKG::resizeEvent(QResizeEvent *event)
{
    ui->panSKG->ensureVisible(QRectF(-m_diap, -m_diap, m_diap * 2, m_diap * 2));
    QWidget::resizeEvent(event);
}

void AreaSKG::setAreaSKG()
{
    ui->panSKG->setScene(m_sceneSKG);
    m_sceneSKG->addItem(m_gridSKG);
    m_sceneSKG->addItem(m_traceSKG);
    m_sceneSKG->addItem(m_lineSKG);
}
