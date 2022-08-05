#include "diagspectr.h"
#include "ui_diagspectr.h"

#include <QDebug>
#include <QPainter>

#include "baseutils.h"

DiagSpectr::DiagSpectr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DiagSpectr)
{
    ui->setupUi(this);
}

DiagSpectr::~DiagSpectr()
{
    delete ui;
}

void DiagSpectr::addValue(const double value)
{
    m_data << value;
}

void DiagSpectr::setFormatData(const int frequency, const int points, const double maxFrequency)
{
    m_frequency = frequency;
    m_points = points;
    m_maxFrequency = maxFrequency;
    m_maxFreq = maxFrequency;
}

void DiagSpectr::setVisualArea(const double minFreq, const double maxFreq, const double minValue, const double maxValue)
{
    m_minFreq = minFreq;
    m_maxFreq = maxFreq;
    m_minValue = minValue;
    m_maxValue = maxValue;
}

void DiagSpectr::resetVisualArea()
{
    m_minFreq = 0;
    m_maxFreq = m_maxFrequency;
    m_minValue = -1;
    m_maxValue = -1;
}

void DiagSpectr::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    //! Фон
    auto backColor = palette().background().color();
    painter.setBrush(QBrush(backColor , Qt::SolidPattern));
    painter.setPen(QPen(backColor , 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawRect(geometry());

    //! Оси
    painter.setPen(QPen(m_colorAxis, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawLine(m_bounds.x(), m_bounds.y(), m_bounds.x(), geometry().height() - m_bounds.bottom());
    painter.drawLine(m_bounds.x(), geometry().height() - m_bounds.bottom(),
                     geometry().width() - m_bounds.right(), geometry().height() - m_bounds.bottom());

    //! Заголовок
    painter.setFont(QFont("Sans", 10, QFont::Bold, false));
    auto size = BaseUtils::getTextSize(&painter, m_title);
    painter.drawText(width() / 2 - size.width() / 2, 16, m_title);

    //! Названия осей
    painter.setFont(QFont("Sans", 10, 0, false));
    painter.drawText(5, 15, m_nameAxisY);
    size = BaseUtils::getTextSize(&painter, m_nameAxisX);
    painter.drawText(width() - size.width() - 5, height() - size.height() + 5, m_nameAxisX);

    //! Пропорция абсцисс
    double propX = static_cast<double>(width() - m_bounds.x() - m_bounds.right()) / 6.0;
    if (m_minFreq > 0 && m_maxFreq < m_maxFrequency && m_maxFreq > m_minFreq)
        propX = static_cast<double>(width() - m_bounds.x() - m_bounds.right()) / (m_maxFreq - m_minFreq);

    //! Пропорция ординат
    double max = 0;
    foreach (auto v, m_data)
        if (v > max)
            max = v;
    double propY = static_cast<double>(height() - m_bounds.top() - m_bounds.bottom()) / max;
    if (m_minValue > -1 && m_maxValue > -1 && m_maxValue > m_minValue)
        propY = static_cast<double>(height() - m_bounds.top() - m_bounds.bottom()) / (m_maxValue - m_minValue);

    int ox = m_bounds.x();
    int ov = height() - m_bounds.bottom();
    for (int i = 0; i < m_data.size(); ++i)
    {
        double freq = static_cast<double>(i * m_frequency) / static_cast<double>(m_points);

        if (freq > m_minFreq && freq < m_maxFreq)
        {

            painter.setBrush(QBrush(m_line1Color, Qt::SolidPattern));
            painter.setPen(QPen(m_line1Color, 1, Qt::SolidLine, Qt::FlatCap));

            int x = static_cast<int>(m_bounds.x() + freq * propX);
            int v = static_cast<int>(height() - m_bounds.bottom() - m_data.at(i) * propY);
            painter.drawLine(x, height() - m_bounds.bottom(), x, v);
            painter.drawLine(ox, ov, x, v);

            ox = x;
            ov = v;
        }
    }
}
