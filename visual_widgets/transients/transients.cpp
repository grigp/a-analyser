#include "transients.h"
#include "ui_transients.h"

#include <QPainter>

namespace
{
static const int FieldH = 20;  ///< Размер поля для горизонтальной шкалы
static const int FieldV = 36;  ///< Размер поля для вертикальной шкалы

static double comp_min = INT_MAX;
static double comp_max = -INT_MAX;
static double ret_min = INT_MAX;
static double ret_max = -INT_MAX;

}

Transients::Transients(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Transients)
{
    ui->setupUi(this);
}

Transients::~Transients()
{
    delete ui;
}

void Transients::beginAddValues()
{
    m_compensation.clear();
    m_return.clear();
    comp_min = INT_MAX;
    comp_max = -INT_MAX;
    ret_min = INT_MAX;
    ret_max = -INT_MAX;
}

void Transients::endAddValues()
{
    update();
}

void Transients::setCompensationValue(const double value)
{
    m_compensation.append(value);
    if (value > comp_max) comp_max = value;
    if (value < comp_min) comp_min = value;
}

void Transients::setReturnValue(const double value)
{
    m_return.append(value);
    if (value > ret_max) ret_max = value;
    if (value < ret_min) ret_min = value;
}

void Transients::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    painter.setBrush(QBrush(m_backgroundColor, Qt::SolidPattern));
    painter.setPen(QPen(m_backgroundColor, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawRect(geometry());

    //! Координата Y нижней линии (Низ для графика возврата)
    int lineRetY = geometry().height() - FieldH;
    //! Координата Y верхней линии (Низ для графика компенсации)
    int lineCompY = (lineRetY) / 2;
    //! Высота зоны для одного графика
    int areaSize = lineCompY;

    //! Оси
    painter.setPen(QPen(m_axisColor, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawLine(4, lineRetY, geometry().width(), lineRetY);
    painter.drawLine(4, lineCompY, geometry().width(), lineCompY);
    painter.drawLine(FieldV, lineRetY, FieldV, 0);

    //! Если заданы корректные данные, то строим график
    if (comp_min < comp_max && ret_min < ret_max &&
        m_compensation.size() > 0 && m_return.size() > 0 &&
        m_compensation.size() == m_return.size())
    {
        //! Пропорции построения графиков
        double minComp = 0;
        if (comp_min < 0) minComp = comp_min;
        double maxComp = 100;
        if (comp_max > 100) maxComp = comp_max;
        double minRet = 0;
        if (ret_min < 0) minRet = ret_min;
        double maxRet = 100;
        if (ret_max > 100) maxRet = ret_max;
        double propComp = 1;
        if (maxComp - minComp > 0) propComp = areaSize / (maxComp - minComp);
        double propRet = 1;
        if (maxRet - minRet > 0) propRet = areaSize / (maxRet - minRet);

        //! Линии 0% и 100%
        painter.setPen(QPen(m_axisColor, 1, Qt::DashLine, Qt::FlatCap));
        int lineComp0 = static_cast<int>(lineCompY - (-minComp) * propComp);
        int lineComp100 = static_cast<int>(lineCompY - (100-minComp) * propComp);
        painter.drawLine(FieldV, lineComp0, geometry().width(), lineComp0);
        painter.drawLine(FieldV, lineComp100, geometry().width(), lineComp100);
        painter.drawText(6, lineComp0 + 4, "0%");
        painter.drawText(1, lineComp100 + 4, "100%");
        int lineRet0 = static_cast<int>(lineRetY - (-minRet) * propRet);
        int lineRet100 = static_cast<int>(lineRetY - (100-minRet) * propRet);
        painter.drawLine(FieldV, lineRet0, geometry().width(), lineRet0);
        painter.drawLine(FieldV, lineRet100, geometry().width(), lineRet100);
        painter.drawText(6, lineRet0 + 4, "0%");
        painter.drawText(1, lineRet100 + 4, "100%");

        double step = static_cast<double>(geometry().width() - FieldV) / static_cast<double>(m_compensation.size());
        for (int i = 0; i < m_compensation.size() - 1; ++i)
        {
            int x1 = static_cast<int>(FieldV + i * step);
            int x2 = static_cast<int>(FieldV + (i + 1) * step);

            if (i < m_latentComp * m_freq)
                painter.setPen(QPen(m_latentColor, 1, Qt::SolidLine, Qt::FlatCap));
            else
            if (i < m_swingComp * m_freq)
                painter.setPen(QPen(m_swingColor, 1, Qt::SolidLine, Qt::FlatCap));
            else
                painter.setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::FlatCap));

            int y1 = static_cast<int>(lineComp0 - m_compensation[i] * propComp);
            int y2 = static_cast<int>(lineComp0 - m_compensation[i + 1] * propComp);
            painter.drawLine(x1, y1, x2, y2);

            if (i < m_latentRet * m_freq)
                painter.setPen(QPen(m_latentColor, 1, Qt::SolidLine, Qt::FlatCap));
            else
            if (i < m_swingRet * m_freq)
                painter.setPen(QPen(m_swingColor, 1, Qt::SolidLine, Qt::FlatCap));
            else
                painter.setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::FlatCap));

            y1 = static_cast<int>(lineRet0 - m_return[i] * propRet);
            y2 = static_cast<int>(lineRet0 - m_return[i + 1] * propRet);
            painter.drawLine(x1, y1, x2, y2);

            //! Секундные метки
            if ((i % m_freq) == 0 )
            {
                painter.setPen(QPen(m_axisColor, 1, Qt::DotLine, Qt::FlatCap));
                painter.drawLine(x1, 0, x1, lineRetY);
                painter.drawText(x1 - 4, lineRetY + 15, QString::number(i / m_freq));

            }
        }

        //! Метки латентного периода
        painter.setPen(QPen(m_latentColor, 1, Qt::DashLine, Qt::FlatCap));
        int x = static_cast<int>(FieldV + m_latentComp * m_freq * step);
        painter.drawLine(x, 0, x, lineCompY);
        x = static_cast<int>(FieldV + m_latentRet * m_freq * step);
        painter.drawLine(x, lineCompY, x, lineRetY);

        //! Метки броска
        painter.setPen(QPen(m_swingColor, 1, Qt::DashLine, Qt::FlatCap));
        x = static_cast<int>(FieldV + m_swingComp * m_freq * step);
        painter.drawLine(x, 0, x, lineCompY);
        x = static_cast<int>(FieldV + m_swingRet * m_freq * step);
        painter.drawLine(x, lineCompY, x, lineRetY);
    }

}
