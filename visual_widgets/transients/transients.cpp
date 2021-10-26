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

void Transients::params(const StepOffsetDefines::Stage stage,
                        double &latent, double &swing, double &spurt, double &reaction, double &statism, double &deviation) const
{
    if (stage == StepOffsetDefines::stgCompensaton)
    {
        latent = m_parComp.latent;
        swing = m_parComp.swing;
        spurt = m_parComp.spurt;
        reaction = m_parComp.reaction;
        statism = m_parComp.statism;
        deviation = m_parComp.deviation;
    }
    else
    if (stage == StepOffsetDefines::stgReturn)
    {
        latent = m_parRet.latent;
        swing = m_parRet.swing;
        spurt = m_parRet.spurt;
        reaction = m_parRet.reaction;
        statism = m_parRet.statism;
        deviation = m_parRet.deviation;
    }
}

void Transients::setParams(const StepOffsetDefines::Stage stage,
                           const double latent, const double swing, const double spurt, const
                           double reaction, const double statism, const double deviation)
{
    if (stage == StepOffsetDefines::stgCompensaton)
    {
        m_parComp.latent = latent;
        m_parComp.swing = swing;
        m_parComp.spurt = spurt;
        m_parComp.reaction = reaction;
        m_parComp.statism = statism;
        m_parComp.deviation = deviation;
    }
    else
    if (stage == StepOffsetDefines::stgReturn)
    {
        m_parRet.latent = latent;
        m_parRet.swing = swing;
        m_parRet.spurt = spurt;
        m_parRet.reaction = reaction;
        m_parRet.statism = statism;
        m_parRet.deviation = deviation;
    }
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

    auto backColor = palette().background().color();
    painter.setBrush(QBrush(backColor, Qt::SolidPattern));
    painter.setPen(QPen(backColor, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawRect(geometry());

    //! Координата Y нижней линии (Низ для графика возврата)
    int lineRetY = geometry().height() - FieldH;
    //! Координата Y верхней линии (Низ для графика компенсации)
    int lineCompY = (lineRetY) / 2;
    //! Высота зоны для одного графика
    int areaSize = lineCompY;

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
        double step = static_cast<double>(geometry().width() - FieldV) / static_cast<double>(m_compensation.size());

        //! Уровень линий 0% и 100%
        int lineComp0 = static_cast<int>(lineCompY - (-minComp) * propComp);
        int lineComp100 = static_cast<int>(lineCompY - (100-minComp) * propComp);
        int lineRet0 = static_cast<int>(lineRetY - (-minRet) * propRet);
        int lineRet100 = static_cast<int>(lineRetY - (100-minRet) * propRet);

        //! Выделение зоны удержания
        painter.setBrush(QBrush(m_retentionColor, Qt::SolidPattern));
        painter.setPen(QPen(m_retentionColor, 1, Qt::SolidLine, Qt::FlatCap));
        int x = static_cast<int>(FieldV + m_parComp.reaction * m_freq * step);
        int y1 = static_cast<int>(lineComp0 - (100 + m_parComp.statism + 3 * m_parComp.deviation) * propComp);
        int h = static_cast<int>(2 * 3 *m_parComp.deviation * propComp);
        painter.drawRect(x, y1, geometry().width() - x, h);
        x = static_cast<int>(FieldV + m_parRet.reaction * m_freq * step);
        y1 = static_cast<int>(lineRet0 - (100 + m_parRet.statism + 3 * m_parRet.deviation) * propRet);
        h = static_cast<int>(2 * 3 * m_parRet.deviation * propRet);
        painter.drawRect(x, y1, geometry().width() - x, h);
        painter.setBrush(QBrush(backColor, Qt::SolidPattern));

        //! Оси
        painter.setPen(QPen(m_axisColor, 1, Qt::SolidLine, Qt::FlatCap));
        painter.drawLine(4, lineRetY, geometry().width(), lineRetY);
        painter.drawLine(4, lineCompY, geometry().width(), lineCompY);
        painter.drawLine(FieldV, lineRetY, FieldV, 0);

        //! Линии 0% и 100%
        painter.setPen(QPen(m_axisColor, 1, Qt::DashLine, Qt::FlatCap));
        painter.drawLine(FieldV, lineComp0, geometry().width(), lineComp0);
        painter.drawLine(FieldV, lineComp100, geometry().width(), lineComp100);
        painter.drawText(6, lineComp0 + 4, "0%");
        painter.drawText(1, lineComp100 + 4, "100%");

        painter.drawLine(FieldV, lineRet0, geometry().width(), lineRet0);
        painter.drawLine(FieldV, lineRet100, geometry().width(), lineRet100);
        painter.drawText(6, lineRet0 + 4, "0%");
        painter.drawText(1, lineRet100 + 4, "100%");

        //! Графики
        for (int i = 0; i < m_compensation.size() - 1; ++i)
        {
            int x1 = static_cast<int>(FieldV + i * step);
            int x2 = static_cast<int>(FieldV + (i + 1) * step);

            //! Компенсация
            painter.setPen(QPen(getColorTransient(i, m_parComp), 2, Qt::SolidLine, Qt::FlatCap));
            int y1 = static_cast<int>(lineComp0 - m_compensation[i] * propComp);
            int y2 = static_cast<int>(lineComp0 - m_compensation[i + 1] * propComp);
            painter.drawLine(x1, y1, x2, y2);

            //! Возврат
            painter.setPen(QPen(getColorTransient(i, m_parRet), 2, Qt::SolidLine, Qt::FlatCap));
            y1 = static_cast<int>(lineRet0 - m_return[i] * propRet);
            y2 = static_cast<int>(lineRet0 - m_return[i + 1] * propRet);
            painter.drawLine(x1, y1, x2, y2);

            //! Секундные метки
            if ((i % m_freq) == 0)
            {
                painter.setPen(QPen(m_axisColor, 1, Qt::DotLine, Qt::FlatCap));
                painter.drawLine(x1, 0, x1, lineRetY);
                painter.drawText(x1 - 4, lineRetY + 15, QString::number(i / m_freq));
            }
        }

        //! Метки латентного периода
        painter.setPen(QPen(m_latentColor, 1, Qt::DashLine, Qt::FlatCap));
        x = static_cast<int>(FieldV + m_parComp.latent * m_freq * step);
        painter.drawLine(x, 0, x, lineCompY);
        x = static_cast<int>(FieldV + m_parRet.latent * m_freq * step);
        painter.drawLine(x, lineCompY, x, lineRetY);

        //! Метки размаха
        painter.setPen(QPen(m_swingColor, 1, Qt::DashLine, Qt::FlatCap));
        x = static_cast<int>(FieldV + m_parComp.swing * m_freq * step);
        painter.drawLine(x, 0, x, lineCompY);
        x = static_cast<int>(FieldV + m_parRet.swing * m_freq * step);
        painter.drawLine(x, lineCompY, x, lineRetY);

        //! Метки броска
        painter.setPen(QPen(m_spurtColor, 1, Qt::DashLine, Qt::FlatCap));
        x = static_cast<int>(FieldV + m_parComp.spurt * m_freq * step);
        painter.drawLine(x, 0, x, lineCompY);
        x = static_cast<int>(FieldV + m_parRet.spurt * m_freq * step);
        painter.drawLine(x, lineCompY, x, lineRetY);

        //! Метки времени реакции
        painter.setPen(QPen(m_stabilisationColor, 1, Qt::DashLine, Qt::FlatCap));
        x = static_cast<int>(FieldV + m_parComp.reaction * m_freq * step);
        painter.drawLine(x, 0, x, lineCompY);
        x = static_cast<int>(FieldV + m_parRet.reaction * m_freq * step);
        painter.drawLine(x, lineCompY, x, lineRetY);
    }
}

QColor Transients::getColorTransient(const int cnt, const Transients::Params &params) const
{
    if (cnt < params.latent * m_freq)
        return m_latentColor;
    else
    if (cnt < params.swing * m_freq)
        return m_swingColor;
    else
    if (cnt < params.spurt * m_freq)
        return m_spurtColor;
    else
    if (cnt < params.reaction * m_freq)
        return m_stabilisationColor;
    else
        return Qt::darkGreen;
}
