#include "areaskg.h"
#include "ui_areaskg.h"

#include "gridskg.h"
#include "traceskg.h"

AreaSKG::AreaSKG(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AreaSKG)
  , m_sceneSKG(new QGraphicsScene(QRectF(-128, -128, 256, 256)))
  , m_gridSKG(new GridSKG(128))
  , m_traceSKG(new TraceSKG(128))
{
    ui->setupUi(this);
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
    m_diap = diap;
}

void AreaSKG::setMarker(const double x, const double y)
{
    int minS = qMin(ui->panSKG->width(), ui->panSKG->height());
    double prop = static_cast<double>(minS / 2) / static_cast<double>(m_diap);

    m_marker->setPos(x * prop - m_marker->boundingRect().width() / 2,
                     - y * prop - m_marker->boundingRect().height() / 2);
}

void AreaSKG::setAreaSKG()
{
    ui->panSKG->setScene(m_sceneSKG);
    m_sceneSKG->addItem(m_gridSKG);
    m_sceneSKG->addItem(m_traceSKG);
    m_marker = m_sceneSKG->addRect(QRectF(0, 0, 10, 10), QPen(Qt::darkRed), QBrush(Qt::red));
}
