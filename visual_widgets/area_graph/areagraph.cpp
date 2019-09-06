#include "areagraph.h"
#include "ui_areagraph.h"

#include "signalaccess.h"
#include "baseutils.h"

#include <QPainter>
#include <QDebug>


namespace  {
///< Отступы от краев виджета
static const int LeftSpace = 50;
static const int RightSpace = 10;
static const int TopSpace = 5;
static const int BottomSpace = 20;

static const QVector<QColor> PaletteDefault = {Qt::darkCyan, Qt::red, Qt::blue, Qt::green, Qt::darkYellow, Qt::darkMagenta};

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
    m_areases.append(new GraphArea(signal, numSubChan, tr("ФДС")));
    update();
}

GraphArea *AreaGraph::area(const int areaNum) const
{
    Q_ASSERT(areaNum >= 0 && areaNum < m_areases.size());
    return m_areases.at(areaNum);
}

void AreaGraph::clear()
{
    foreach (auto* area, m_areases)
        delete area;
    m_areases.clear();
    update();
}

void AreaGraph::setColorBackground(const QColor &color)
{
    m_envColors.colorBackground = color;
    update();
}

void AreaGraph::setColorAxis(const QColor &color)
{
    m_envColors.colorAxis = color;
    update();
}

void AreaGraph::setColorGrid(const QColor &color)
{
    m_envColors.colorGrid = color;
    update();
}

void AreaGraph::setColorLabels(const QColor &color)
{
    m_envColors.colorLabels = color;
    update();
}

void AreaGraph::setXCoordSignalMode(const AreaGraph::XCoordSignalMode mode)
{
    m_xcsm = mode;
    update();
}

void AreaGraph::setStartPoint(const int startPoint)
{
    m_startPoint = startPoint;
    update();
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

        //! По зонам построения
        for (int iz = 0; iz < m_areases.size(); ++iz)
        {
            //! Ось зоны
            painter.setPen(QPen(m_envColors.colorAxis, 1, Qt::SolidLine, Qt::FlatCap));
            int axisY = TopSpace + (iz + 1) * zoneH;
            painter.drawLine(LeftSpace, axisY, width() - RightSpace, axisY);

            //! Пропорции по значениям и шаг
            double prop = zoneH / (m_areases.at(iz)->maxValue() - m_areases.at(iz)->minValue());
            double step = 1;
            if (m_xcsm == xsm_fullSignal)
                step = static_cast<double>(width() - LeftSpace - RightSpace) /
                       static_cast<double>(m_areases.at(iz)->signal()->size());

            //! Минимальное значение
            QString sMin = QString::number(trunc(m_areases.at(iz)->minValue()));
            auto size = BaseUtils::getTextSize(&painter, sMin);
            painter.drawText(LeftSpace - size.width() - 5, height() - BottomSpace - 3, sMin);

            //! Максимальное значение
            QString sMax = QString::number(trunc(m_areases.at(iz)->maxValue()));
            size = BaseUtils::getTextSize(&painter, sMax);
            painter.drawText(LeftSpace - size.width() - 5, TopSpace + size.height(), sMax);

            //! Название зоны
            size = BaseUtils::getTextSize(&painter, m_areases.at(iz)->name());
            painter.drawText(LeftSpace + 5, TopSpace + size.height(), m_areases.at(iz)->name());

            //! Линия нуля
            if (m_areases.at(iz)->maxValue() > 0 && m_areases.at(iz)->minValue() < 0)
            {
                int line0 = trunc(axisY - (0 - m_areases.at(iz)->minValue()) * prop);
                painter.drawLine(LeftSpace, line0, width() - RightSpace, line0);
                auto size = BaseUtils::getTextSize(&painter, "0");
                painter.drawText(LeftSpace - size.width() - 5, line0 + size.height()/2, "0");
            }

            //! По сигналу
            for (int i = m_startPoint; i < m_areases.at(iz)->signal()->size() - 1; ++i)
            {
                int x1 = LeftSpace + trunc(i * step);
                int x2 = LeftSpace + trunc((i + 1) * step);
                int y1 = axisY - trunc((m_areases.at(iz)->signal()->value(0, i) - m_areases.at(iz)->minValue()) * prop);
                int y2 = axisY - trunc((m_areases.at(iz)->signal()->value(0, i + 1) - m_areases.at(iz)->minValue()) * prop);
                painter.setPen(QPen(m_areases.at(iz)->color(0), 1, Qt::SolidLine, Qt::FlatCap));
                painter.drawLine(x1, y1, x2, y2);

                if (iz == 0)
                {
                    if (i % m_areases.at(iz)->signal()->frequency() == 0 && i != 0)
                    {
                        painter.setPen(QPen(m_envColors.colorGrid, 1, Qt::DotLine, Qt::FlatCap));
                        painter.drawLine(x1, TopSpace, x1, height() - BottomSpace);
                        QString s = QString::number(i / m_areases.at(iz)->signal()->frequency());
                        auto size = BaseUtils::getTextSize(&painter, s);
                        painter.setPen(QPen(m_envColors.colorAxis, 1, Qt::SolidLine, Qt::FlatCap));
                        painter.drawText(x1 - size.width() / 2, height() - BottomSpace + size.height() + 1, s);
                    }
                }
            }
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
GraphArea::GraphArea(SignalAccess *signal,
                     const int numSubChan,
                     const QString &name)
    : m_signal(signal)
    , m_numSubChan(numSubChan)
    , m_name(name)
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

void GraphArea::setDiapazone(const double minVal, const double maxVal)
{
    m_minVal = minVal;
    m_maxVal = maxVal;
}
