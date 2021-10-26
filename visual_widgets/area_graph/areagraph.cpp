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

static const QVector<QColor> PaletteDefault = {Qt::darkGreen, Qt::blue, Qt::darkCyan, Qt::red, Qt::darkYellow, Qt::darkMagenta};

///< Шаг секундных меток
enum LabelStep
{
      lsOne = 1     ///< Каждую секунду
    , lsFive = 5    ///< Каждые пять секунд
    , lsTen = 10    ///< Каждые десять секунд
    , lsSixty = 60  ///< Каждую минуту
};

}

AreaGraph::AreaGraph(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AreaGraph)
{
    m_envColors.colorAxis = Qt::black;
    m_envColors.colorGrid = Qt::darkGray;
    m_envColors.colorLabels = Qt::black;
    m_envColors.colorCursor = Qt::darkGray;
    m_envColors.colorMarkers = Qt::darkRed;
    m_envColors.colorFillBetweenSubchans = Qt::cyan;

    m_areases.clear();
    ui->setupUi(this);

}

AreaGraph::~AreaGraph()
{
    delete ui;
}

void AreaGraph::appendSignal(SignalAccess *signal, const QString &name, const int numSubChan)
{
    m_areases.append(new GraphArea(this, signal, numSubChan, name));
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

void AreaGraph::setColorCursor(const QColor &color)
{
    m_envColors.colorCursor = color;
    update();
}

void AreaGraph::setColorMarker(const QColor &color)
{
    m_envColors.colorMarkers = color;
    update();
}

void AreaGraph::setColorFillBetweenSubchans(const QColor &color)
{
    m_envColors.colorFillBetweenSubchans = color;
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

void AreaGraph::setHScale(const int hScale)
{
    m_hScale = hScale;
    update();
}

void AreaGraph::setIsZeroing(const bool zeroing)
{
    m_isZeroing = zeroing;
    update();
}

void AreaGraph::setDiapazone(const int numArea, const double minVal, const double maxVal)
{
    Q_ASSERT(numArea >= 0 && numArea < m_areases.size());
    m_areases.at(numArea)->setDiapazone(minVal, maxVal);
    update();
}

void AreaGraph::setDiapazone(const double minVal, const double maxVal)
{
    foreach (auto area, m_areases)
        area->setDiapazone(minVal, maxVal);
    update();
}

int AreaGraph::markerCount(const int numArea) const
{
    Q_ASSERT(numArea >= 0 && numArea < m_areases.size());
    return m_areases.at(numArea)->markerCount();
}

MarkerInfo AreaGraph::marker(const int numArea, const int idx) const
{
    Q_ASSERT(numArea >= 0 && numArea < m_areases.size());
    return m_areases.at(numArea)->marker(idx);
}

void AreaGraph::addMarker(const int numArea, const int pos, const QString comment)
{
    Q_ASSERT(numArea >= 0 && numArea < m_areases.size());
    m_areases.at(numArea)->addMarker(pos, comment);
}

void AreaGraph::setCursor(const int numArea, const int pos)
{
    Q_ASSERT(numArea >= 0 && numArea < m_areases.size());
    m_areases.at(numArea)->setCursor(pos);
    update();
}

bool AreaGraph::isFillBetweenSubchans(const int numArea) const
{
    Q_ASSERT(numArea >= 0 && numArea < m_areases.size());
    return m_areases.at(numArea)->isFillBetweenSubchans();
}

void AreaGraph::setIsFillBetweenSubchans(const int numArea, const bool isFill)
{
    Q_ASSERT(numArea >= 0 && numArea < m_areases.size());
    m_areases.at(numArea)->setIsFillBetweenSubchans(isFill);
}

void AreaGraph::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.save();

    //! Фон
    auto backColor = palette().background().color();
    painter.setBrush(QBrush(backColor, Qt::SolidPattern));
    painter.setPen(QPen(backColor, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawRect(0, 0, width(), height());

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
            //! Определение шага секундных меток
            double stepMark = step * m_areases.at(iz)->signal()->frequency();
            int secCount = m_areases.at(iz)->signal()->size() / m_areases.at(iz)->signal()->frequency();
            QString s = QString::number(secCount);
            auto sizeOne = BaseUtils::getTextSize(&painter, s);
            LabelStep ls = lsOne;
            if ((stepMark < (sizeOne.width() + 7)) && (stepMark * 5 >= (sizeOne.width() + 7)))
                ls = lsFive;
            else
            if ((stepMark * 5 < (sizeOne.width() + 7)) && (stepMark * 10 >= (sizeOne.width() + 7)))
                ls = lsTen;
            else
            if ((stepMark * 10 < (sizeOne.width() + 7)) && (stepMark * 60 >= (sizeOne.width() + 7)))
                ls = lsSixty;

            //! Минимальное значение
            QString sMin = QString::number(trunc(m_areases.at(iz)->minValue()));
            auto size = BaseUtils::getTextSize(&painter, sMin);
            painter.drawText(LeftSpace - size.width() - 5, axisY - 3, sMin);

            //! Максимальное значение
            QString sMax = QString::number(trunc(m_areases.at(iz)->maxValue()));
            size = BaseUtils::getTextSize(&painter, sMax);
            painter.drawText(LeftSpace - size.width() - 5, axisY - zoneH + size.height(), sMax);

            //! Название зоны
            size = BaseUtils::getTextSize(&painter, m_areases.at(iz)->name());
            painter.drawText(LeftSpace + 5, axisY - zoneH + size.height(), m_areases.at(iz)->name());

            //! Линия нуля
            if (m_areases.at(iz)->maxValue() > 0 && m_areases.at(iz)->minValue() < 0)
            {
                int line0 = static_cast<int>(axisY - (0 - m_areases.at(iz)->minValue()) * prop);
                painter.drawLine(LeftSpace, line0, width() - RightSpace, line0);
                auto size = BaseUtils::getTextSize(&painter, "0");
                painter.drawText(LeftSpace - size.width() - 5, line0 + size.height()/2, "0");
            }

            //! Настройка растяжения и позиции в зависимости от установок
            int hScale = 1;
            int startPoint = 0;
            if (m_xcsm == xsm_scrolling)
            {
                hScale = m_hScale;
                startPoint = m_startPoint;
            }
            //! По сигналу
            for (int i = 0; i < m_areases.at(iz)->signal()->size() - 1; ++i)
            {
                int x1 = LeftSpace + static_cast<int>((i - startPoint) * step * hScale);
                int x2 = LeftSpace + static_cast<int>((i - startPoint + 1) * step * hScale);

                if (i > startPoint)
                {
                    if (x1 < LeftSpace)
                        x1 = LeftSpace;

                    //! Лямбда функция вывода линии.
                    //! Необходима, чтобы не передавать кучу параметров в приватный метод
                    auto drawLine = [&](const int chan1, const int chan2, const QColor color)
                    {
                        double v1 = m_areases.at(iz)->signal()->value(chan1, i);
                        double v2 = m_areases.at(iz)->signal()->value(chan2, i + 1);
                        if (m_isZeroing)
                        {
                            v1 = v1 - m_areases.at(iz)->average(chan1);
                            v2 = v2 - m_areases.at(iz)->average(chan2);
                        }
                        int y1 = axisY - static_cast<int>((v1 - m_areases.at(iz)->minValue()) * prop);
                        int y2 = axisY - static_cast<int>((v2 - m_areases.at(iz)->minValue()) * prop);
                        painter.setPen(QPen(color, 1, Qt::SolidLine, Qt::FlatCap));
                        painter.drawLine(x1, y1, x2, y2);
                    };

                    //! Межканальная заливка. Только для первых двух каналов
                    if (m_areases.at(iz)->isFillBetweenSubchans())
                        drawLine(0, 1, m_envColors.colorFillBetweenSubchans);

                    //! Все подканалы
                    if (m_areases.at(iz)->numSubChan() == -1)
                        //! По подканалам
                        for (int j = 0; j < m_areases.at(iz)->signal()->subChansCount(); ++j)
                            drawLine(j, j, m_areases.at(iz)->color(j));
                    else
                    //! Только выбранный подканал
                    if (m_areases.at(iz)->numSubChan() >= 0 &&
                        m_areases.at(iz)->numSubChan() < m_areases.at(iz)->signal()->subChansCount())
                        drawLine(m_areases.at(iz)->numSubChan(),
                                 m_areases.at(iz)->numSubChan(),
                                 m_areases.at(iz)->color(m_areases.at(iz)->numSubChan()));

                    //! Секундные метки
                    if (iz == 0)
                    {
                        if (i != 0)
                        {
                            if ((ls == lsOne && i % m_areases.at(iz)->signal()->frequency() == 0) ||
                                (ls == lsFive && i % (m_areases.at(iz)->signal()->frequency() * 5) == 0) ||
                                (ls == lsTen && i % (m_areases.at(iz)->signal()->frequency() * 10) == 0) ||
                                (ls == lsSixty && i % (m_areases.at(iz)->signal()->frequency() * 60) == 0))
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

            //! Курсор в зоне
            if (m_areases.at(iz)->cursorPos() > -1)
            {
                int x = LeftSpace + static_cast<int>((m_areases.at(iz)->cursorPos() - startPoint) * step * hScale);
                painter.setPen(QPen(m_envColors.colorCursor, 1, Qt::SolidLine, Qt::FlatCap));
                painter.drawLine(x, axisY, x, axisY - zoneH);
            }

            //! Маркеры
            painter.setPen(QPen(m_envColors.colorMarkers, 1, Qt::SolidLine, Qt::FlatCap));
            for (int i = 0; i < m_areases.at(iz)->markerCount(); ++i)
            {
                int x = LeftSpace + static_cast<int>((m_areases.at(iz)->marker(i).position - startPoint) * step * hScale);
                painter.drawLine(x, axisY, x, axisY - zoneH);
                painter.drawText(x + 5, axisY - 5, m_areases.at(iz)->marker(i).comment);
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
GraphArea::GraphArea(QWidget* parent,
                     SignalAccess *signal,
                     const int numSubChan,
                     const QString &name)
    : m_parent(parent)
    , m_signal(signal)
    , m_numSubChan(numSubChan)
    , m_name(name)
    , m_palette(QVector<QColor>() << static_cast<AreaGraph*>(m_parent)->line1Color()
                                  << static_cast<AreaGraph*>(m_parent)->line2Color()
                                  << static_cast<AreaGraph*>(m_parent)->line3Color()
                                  << static_cast<AreaGraph*>(m_parent)->line4Color()
                                  << static_cast<AreaGraph*>(m_parent)->line5Color()
                                  << static_cast<AreaGraph*>(m_parent)->line6Color())
//    , m_palette(PaletteDefault)
{
    computeAverageValue();
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

double GraphArea::average(const int numSubChan)
{
    return m_average.at(numSubChan);
}

void GraphArea::setDiapazone(const double minVal, const double maxVal)
{
    m_minVal = minVal;
    m_maxVal = maxVal;
}

void GraphArea::setCursor(const int pos)
{
    m_cursorPos = pos;
}

MarkerInfo GraphArea::marker(const int idx) const
{
    Q_ASSERT(idx >=0 && idx < m_markers.size());
    return m_markers.at(idx);
}

void GraphArea::addMarker(const int pos, const QString comment)
{
    MarkerInfo mi;
    mi.position = pos;
    mi.comment = comment;
    m_markers.append(mi);
}

void GraphArea::computeAverageValue()
{
    m_average.clear();
    for (int i = 0; i < m_signal->subChansCount(); ++i)
    {
        double mid = 0;
        for (int j = 0; j < m_signal->size(); ++j)
        {
            double v = m_signal->value(i, j);
            mid = mid + v;
        }
        m_average.append(mid / static_cast<double>(m_signal->size()));
    }
}
