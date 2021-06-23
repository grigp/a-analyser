#include "transients.h"
#include "ui_transients.h"

#include <QPainter>

namespace
{
static const int FieldH = 20;  ///< Размер поля для горизонтальной шкалы
static const int FieldV = 40;  ///< Размер поля для вертикальной шкалы

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

    //! Координата Y нижней линии (0 для графика возврата)
    int line2Y = geometry().height() - FieldH;
    //! Координата Y верхней линии (0 для графика компенсации)
    int line1Y = (line2Y) / 2;
    //! Высота зоны для одного графика
    int areaSize = line1Y;

    //! Оси
    painter.setPen(QPen(m_axisColor, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawLine(4, line2Y, geometry().width(), line2Y);
    painter.drawLine(4, line1Y, geometry().width(), line1Y);
    painter.drawLine(FieldV, line2Y, FieldV, 0);

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

        double step = (geometry().width() - FieldV) / m_compensation.size();

        for (int i = 0; i < m_compensation.size() - 1; ++i)
        {
            int x1 = static_cast<int>(FieldV + i * step);
            int x2 = static_cast<int>(FieldV + (i + 1) * step);

            int y1 = static_cast<int>(line1Y - m_compensation[i] * propComp);
            int y2 = static_cast<int>(line1Y - m_compensation[i + 1] * propComp);
            painter.drawLine(x1, y1, x2, y2);

            y1 = static_cast<int>(line2Y - m_return[i] * propRet);
            y2 = static_cast<int>(line2Y - m_return[i + 1] * propRet);
            painter.drawLine(x1, y1, x2, y2);
        }


    }

}
