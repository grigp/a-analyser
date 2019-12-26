#include "oscilloscope.h"
#include "ui_oscilloscope.h"

#include "baseutils.h"

#include <QPainter>

namespace  {
///< Отступы от краев виджета
static const int LeftSpace = 50;
static const int RightSpace = 10;
static const int TopSpace = 5;
static const int BottomSpace = 20;

static const QVector<QColor> PaletteDefault = {Qt::darkGreen, Qt::blue, Qt::darkCyan, Qt::red, Qt::darkYellow, Qt::darkMagenta};

}

Oscilloscope::Oscilloscope(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Oscilloscope)
{
    m_envColors.colorAxis = Qt::black;
    m_envColors.colorBackground = Qt::white;
    m_envColors.colorGrid = Qt::darkGray;
    m_envColors.colorLabels = Qt::black;
    m_envColors.colorCursor = Qt::darkGray;

    m_areases.clear();
    ui->setupUi(this);
}

Oscilloscope::~Oscilloscope()
{
    delete ui;
}

void Oscilloscope::appendArea(const QString &areaName, const int channelCount)
{
    m_areases.append(new OscilloscopeArea(areaName, channelCount));
    update();
}

OscilloscopeArea *Oscilloscope::area(const int areaNum) const
{
    Q_ASSERT(areaNum >= 0 && areaNum < m_areases.size());
    return m_areases.at(areaNum);
}

void Oscilloscope::clear()
{
    foreach (auto* area, m_areases)
        delete area;
    m_areases.clear();
    update();
}

void Oscilloscope::setColorBackground(const QColor &color)
{
    m_envColors.colorBackground = color;
    update();
}

void Oscilloscope::setColorAxis(const QColor &color)
{
    m_envColors.colorAxis = color;
    update();
}

void Oscilloscope::setColorGrid(const QColor &color)
{
    m_envColors.colorGrid = color;
    update();
}

void Oscilloscope::setColorLabels(const QColor &color)
{
    m_envColors.colorLabels = color;
    update();
}

void Oscilloscope::setColorCursor(const QColor &color)
{
    m_envColors.colorCursor = color;
    update();
}

void Oscilloscope::setIsZeroing(const bool zeroing)
{
    m_isZeroing = zeroing;
    update();
}

void Oscilloscope::setDiapazone(const int numArea, const double minVal, const double maxVal)
{
    Q_ASSERT(numArea >= 0 && numArea < m_areases.size());
    m_areases.at(numArea)->setDiapazone(minVal, maxVal);
    update();
}

void Oscilloscope::setDiapazone(const double minVal, const double maxVal)
{
    foreach (auto area, m_areases)
        area->setDiapazone(minVal, maxVal);
    update();
}

void Oscilloscope::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.save();

    //! Фон
    painter.setBrush(QBrush(m_envColors.colorBackground, Qt::SolidPattern));
    painter.setPen(QPen(m_envColors.colorBackground, 1, Qt::SolidLine, Qt::FlatCap));
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
                int line0 = trunc(axisY - (0 - m_areases.at(iz)->minValue()) * prop);
                painter.drawLine(LeftSpace, line0, width() - RightSpace, line0);
                auto size = BaseUtils::getTextSize(&painter, "0");
                painter.drawText(LeftSpace - size.width() - 5, line0 + size.height()/2, "0");
            }
        }
    }

    painter.restore();
}

OscilloscopeArea::OscilloscopeArea(const QString &name, const int channelCount)
    : m_name(name)
    , m_channelCount(channelCount)
    , m_palette(PaletteDefault)
{

}

QColor OscilloscopeArea::color(const int colorNum) const
{
    int idx = colorNum % m_palette.size();
    return m_palette.at(idx);
}

void OscilloscopeArea::setColor(const int colorNum, const QColor &color)
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

void OscilloscopeArea::setDiapazone(const double minVal, const double maxVal)
{
    m_minVal = minVal;
    m_maxVal = maxVal;
}
