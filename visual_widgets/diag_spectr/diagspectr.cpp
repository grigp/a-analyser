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

void DiagSpectr::selectArea(QRect rect)
{
    m_selectRect = rect;
    update();
}

QPointF DiagSpectr::getValues(const QPoint point) const
{
    double f = static_cast<double>((point.x() - m_bounds.x()) / m_propX) + m_minFreq;
    double a = - static_cast<double>((point.y() - height() + m_bounds.bottom()) / m_propY);
    if (m_minValue > -1)
        a += m_minValue;
    return QPointF(f, a);
}

QPoint DiagSpectr::getPoint(const QPointF values) const
{
    int x = static_cast<int>(m_bounds.x() + (values.x() - m_minFreq) * m_propX);
    int v = static_cast<int>(height() - m_bounds.bottom() - (values.y() - m_minValue) * m_propY);
    return QPoint(x, v);
}

void DiagSpectr::addFreqArea(const double begin, const double end, const QString name, const QColor colorBackground, const QColor colorText)
{
    m_freqAeases << FreqArea(begin, end, name, colorBackground, colorText);
}

void DiagSpectr::addFreqLabel(const double freq, const QString name)
{
    m_freqLabels << FreqLabel(freq, name);
}

void DiagSpectr::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    //! Определение максимума спектра
    double max = 0;
    double min = 0;
    if (m_minValue > -1 && m_maxValue > -1 && m_maxValue > m_minValue)
    {
        max = m_maxValue - m_minValue;
        min = m_minValue;
    }
    else
        foreach (auto v, m_data)
            if (v > max)
                max = v;
    //! Определение размерности оси ординат для вывода сетки по амплитуде
    int tw = 0;
    QStringList yLabels;
    yLabels.clear();
    painter.setFont(QFont("Sans", 10, 0, false));
    for (int i = 1; i < 10; ++i)
    {
        double v = max / 10 * i;
        QString s = "";
        if (max >= 10)
            s = QString::number(v, 'f', 0);
        else
        if (max < 10 && max >= 3)
            s = QString::number(v, 'f', 1);
        else
        if (max < 3 && max >= 0.5)
            s = QString::number(v, 'f', 2);
        else
        if (max < 0.5 && max >= 0.1)
            s = QString::number(v, 'f', 3);
        else
            s = QString::number(v, 'f', 4);
        yLabels << s;
        auto size = BaseUtils::getTextSize(&painter, s);
        if (size.width() > tw)
            tw = size.width();
    }
    if (tw + 5 > m_bounds.x())
        m_bounds.setX(tw + 5);

    //! Пропорция абсцисс
    double minFreq = 0.0;
    double maxFreq = 6.1;
    m_propX = static_cast<double>(width() - m_bounds.x() - m_bounds.right()) / 6.0;
    if (m_minFreq > 0 && m_maxFreq < m_maxFrequency && m_maxFreq > m_minFreq)
    {
        m_propX = static_cast<double>(width() - m_bounds.x() - m_bounds.right()) / (m_maxFreq - m_minFreq);
        minFreq = m_minFreq;
        maxFreq = m_maxFreq;
    }

    //! Пропорция ординат
    m_propY = static_cast<double>(height() - m_bounds.top() - m_bounds.bottom()) / max;

    //! Фон
    auto backColor = palette().background().color();
    painter.setBrush(QBrush(backColor , Qt::SolidPattern));
    painter.setPen(QPen(backColor , 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawRect(geometry());

    //! Вывод зон по частоте
    foreach (auto area, m_freqAeases)
    {
        painter.setBrush(QBrush(area.colorBackground, Qt::SolidPattern));
        painter.setPen(QPen(area.colorBackground, 1, Qt::SolidLine, Qt::FlatCap));

        int xb = static_cast<int>(m_bounds.x() + (area.begin - minFreq) * m_propX);
        int xe = static_cast<int>(m_bounds.x() + (area.end - minFreq) * m_propX);
        if (xe > m_bounds.x())
        {
            if (xb < m_bounds.x())
                xb = m_bounds.x();
            painter.drawRect(xb, m_bounds.y(), xe, height() - m_bounds.bottom());

            painter.setPen(QPen(area.colorText, 1, Qt::SolidLine, Qt::FlatCap));
            painter.setFont(QFont("Sans", 20, 0, false));
            auto size = BaseUtils::getTextSize(&painter, area.name);
            painter.drawText(xb + 3, m_bounds.y() + size.height() - 5, area.name);
        }
    }

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
    painter.drawText(width() - size.width() - 5, height() - size.height() + 14, m_nameAxisX);

    //! Вывод диаграммы спектра
    painter.setBrush(QBrush(m_line1Color, Qt::SolidPattern));
    painter.setPen(QPen(m_line1Color, 1, Qt::SolidLine, Qt::FlatCap));

    int ox = m_bounds.x();
    int ov = height() - m_bounds.bottom();
    for (int i = 0; i < m_data.size(); ++i)
    {
        double freq = static_cast<double>(i * m_frequency) / static_cast<double>(m_points);

        if (freq > minFreq && freq < maxFreq)
        {

            int x = static_cast<int>(m_bounds.x() + (freq - minFreq) * m_propX);
            int v = static_cast<int>(height() - m_bounds.bottom() - (m_data.at(i) - min) * m_propY);
            painter.drawLine(x, height() - m_bounds.bottom(), x, v);
            painter.drawLine(ox, ov, x, v);

            ox = x;
            ov = v;
        }
    }

    //! Вывод сетки по частоте
    //! propX - кол-во точек на 1 Гц, выводим с шагом 0.1 Гц
    painter.setFont(QFont("Sans", 10, 0, false));
    int i = static_cast<int>(minFreq * 10);
    do {
        int x = static_cast<int>(m_bounds.x() + ((static_cast<double>(i) / 10) - minFreq) * m_propX);
        if (i % 10 == 0)
            painter.setPen(QPen(m_colorGrid, 1, Qt::SolidLine, Qt::FlatCap));
        else
        if (i % 5 == 0)
            painter.setPen(QPen(m_colorGrid, 1, Qt::DashLine, Qt::FlatCap));
        else
            painter.setPen(QPen(m_colorGrid, 1, Qt::DotLine, Qt::FlatCap));

        if (i > 0)
            painter.drawLine(x, m_bounds.top(), x, height() - m_bounds.bottom());

        if (i % 5 == 0)
        {
            QString s = QString::number(static_cast<double>(i) / 10);
            auto size = BaseUtils::getTextSize(&painter, s);
            painter.setPen(QPen(m_colorAxis, 1, Qt::DashLine, Qt::FlatCap));
            painter.drawText(x - size.width() / 2, height() - m_bounds.bottom() + size.height(), s);
        }
        ++i;
    }
    while (i / 10 < maxFreq);

    //! Вывод меток по частоте
    painter.setFont(QFont("Sans", 10, 0, false));
    painter.setPen(QPen(m_colorFreqLabel, 2, Qt::SolidLine, Qt::FlatCap));
    foreach (auto label, m_freqLabels)
    {
        int x = static_cast<int>(m_bounds.x() + (label.freq - minFreq) * m_propX);
        painter.drawLine(x, m_bounds.top(), x, height() - m_bounds.bottom() + 5);
        auto size = BaseUtils::getTextSize(&painter, label.name);
        painter.drawText(x - size.width() / 2, height() - size.height() + 14, label.name);
    }


    //! Вывод сетки по амплитуде. Просто десять значений
    painter.setFont(QFont("Sans", 10, 0, false));
    for (int i = 0; i < yLabels.size(); ++i)
    {
        double v = max / 10 * (i+1);
        int y = static_cast<int>(height() - m_bounds.bottom() - (v - min) * m_propY);
        painter.setPen(QPen(m_colorGrid, 1, Qt::DotLine, Qt::FlatCap));
        painter.drawLine(m_bounds.x(), y, width() - m_bounds.right(), y);

        painter.setPen(QPen(m_colorAxis, 1, Qt::DotLine, Qt::FlatCap));
        painter.drawText(2, y, yLabels.at(i));
    }

    //! Отображение выделенной зоны
    if (m_selectRect != QRect())
    {
        painter.setBrush(QBrush(QColor(0, 0, 0, 125) , Qt::SolidPattern));
        painter.drawRect(m_selectRect);
    }
}

void DiagSpectr::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    emit press(event->x(), event->y());
}

void DiagSpectr::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    emit release(event->x(), event->y());
}

void DiagSpectr::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
    emit move(event->x(), event->y());
}
