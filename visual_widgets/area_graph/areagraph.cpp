#include "areagraph.h"
#include "ui_areagraph.h"

#include <QPainter>

namespace  {
///< Отступы от краев виджета
static const int LeftSpace = 50;
static const int RightSpace = 10;
static const int TopSpace = 5;
static const int BottomSpace = 20;

static const QVector<QColor> PaletteDefault = {Qt::red, Qt::blue, Qt::green, Qt::darkCyan, Qt::darkYellow, Qt::darkMagenta};

}

AreaGraph::AreaGraph(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AreaGraph)
{
    m_envColors.colorAxis = Qt::black;
    m_envColors.colorBackground = Qt::white;
    m_envColors.colorGrid = Qt::darkGray;
    m_envColors.colorLabels = Qt::black;

    m_areases.clear();
    ui->setupUi(this);

}

AreaGraph::~AreaGraph()
{
    delete ui;
}

void AreaGraph::appendSignal(SignalAccess *signal, const int numSubChan)
{
    m_areases.append(new GraphArea(signal, numSubChan));
}

void AreaGraph::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.save();

    //! Фон
    painter.setBrush(QBrush(m_envColors.colorBackground, Qt::SolidPattern));
    painter.setPen(QPen(m_envColors.colorBackground, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawRect(0, 0, width(), height());
//    painter.drawRect(geometry());

    painter.setPen(QPen(m_envColors.colorAxis, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawLine(LeftSpace, TopSpace, LeftSpace, height() - BottomSpace);
    painter.drawLine(LeftSpace, height() - BottomSpace, width() - RightSpace, height() - BottomSpace);

    if (m_areases.size() > 0)
    {
        int zoneH = (height() - TopSpace - BottomSpace) / m_areases.size();

        for (int iz = 0; iz < m_areases.size(); ++iz)
        {
            painter.setPen(QPen(m_envColors.colorAxis, 1, Qt::SolidLine, Qt::FlatCap));
            int axisY = TopSpace + (iz + 1) * zoneH;
            painter.drawLine(LeftSpace, axisY, height() - RightSpace, axisY);
        }
    }

    painter.restore();
}

void AreaGraph::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
}


//! --------------------------------------------------------------------------------------
//! Реализация GraphArea
GraphArea::GraphArea(SignalAccess *signal, const int numSubChan)
    : m_signal(signal)
    , m_numSubChan(numSubChan)
    , m_palette(PaletteDefault)
{

}

QColor GraphArea::color(const int colorNum) const
{
    int idx = colorNum % m_palette.size();
    return m_palette.at(idx);
}

void GraphArea::setColor(const int colorNum, const QColor &color)
{
    Q_ASSERT(colorNum >= 0);
    if (colorNum < m_palette.size())
        m_palette.replace(colorNum, color);
    else
    {
        int ps = m_palette.size();
        for (int i = m_palette.size(); i < colorNum - 1; ++i)
            m_palette.append(m_palette.at(i - ps));
        m_palette.append(color);
    }
}
