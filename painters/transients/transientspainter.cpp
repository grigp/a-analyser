#include "transientspainter.h"

#include <QDebug>

#include "baseutils.h"

namespace
{
static const int FieldH = 20;  ///< Размер поля для горизонтальной шкалы
static const int FieldV = 36;  ///< Размер поля для вертикальной шкалы

static double comp_min = INT_MAX;
static double comp_max = -INT_MAX;
static double ret_min = INT_MAX;
static double ret_max = -INT_MAX;

}

TransientsPainter::TransientsPainter(QPainter *painter, QRect geometry)
    : m_painter(painter), m_geometry(geometry)
{

}

TransientsPainter::TransientsPainter()
{

}

TransientsPainter::~TransientsPainter()
{

}

void TransientsPainter::setCanvas(QPainter *painter, QRect geometry, QWidget *wgt)
{
    m_painter = painter;
    m_geometry = geometry;
    m_widget = wgt;

}

void TransientsPainter::params(const StepOffsetDefines::Stage stage,
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

void TransientsPainter::setParams(const StepOffsetDefines::Stage stage,
                                  const double latent,
                                  const double swing,
                                  const double spurt,
                                  const double reaction,
                                  const double statism,
                                  const double deviation)
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

void TransientsPainter::beginAddValues()
{
    m_compensation.clear();
    m_return.clear();
    comp_min = INT_MAX;
    comp_max = -INT_MAX;
    ret_min = INT_MAX;
    ret_max = -INT_MAX;
}

void TransientsPainter::endAddValues()
{
    doUpdate();
}

void TransientsPainter::setCompensationValue(const double value)
{
    m_compensation.append(value);
    if (value > comp_max) comp_max = value;
    if (value < comp_min) comp_min = value;
}

void TransientsPainter::setReturnValue(const double value)
{
    m_return.append(value);
    if (value > ret_max) ret_max = value;
    if (value < ret_min) ret_min = value;
}

void TransientsPainter::doPaint(const double ratio)
{
    m_painter->save();

    QColor backColor = Qt::white;
    if (m_widget)
        backColor = m_widget->palette().background().color();

    int tw = 8;
    if (ratio > 1)
        tw = static_cast<int>(8 / (ratio / 2));
    m_painter->setFont(QFont("Arial", tw, 0, false));

    int fieldV = static_cast<int>(FieldV * ratio);
    int fieldH = static_cast<int>(FieldH * ratio);

    m_painter->setBrush(QBrush(backColor, Qt::SolidPattern));
    m_painter->setPen(QPen(backColor, 1, Qt::SolidLine, Qt::FlatCap));
    m_painter->drawRect(m_geometry);

    //! Координата Y нижней линии (Низ для графика возврата)
    int lineRetY = m_geometry.top() + m_geometry.height() - fieldH;
    //! Координата Y верхней линии (Низ для графика компенсации)
    int lineCompY = m_geometry.top() + (lineRetY - m_geometry.top()) / 2;
    //! Высота зоны для одного графика
    int areaSize = lineCompY - m_geometry.top();

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
        double step = static_cast<double>(m_geometry.width() - fieldV) / static_cast<double>(m_compensation.size());

        //! Уровень линий 0% и 100%
        int lineComp0 = static_cast<int>(lineCompY - (-minComp) * propComp);
        int lineComp100 = static_cast<int>(lineCompY - (100-minComp) * propComp);
        int lineRet0 = static_cast<int>(lineRetY - (-minRet) * propRet);
        int lineRet100 = static_cast<int>(lineRetY - (100-minRet) * propRet);

        //! Выделение зоны удержания
        m_painter->setBrush(QBrush(m_retentionColor, Qt::SolidPattern));
        m_painter->setPen(QPen(m_retentionColor, 1, Qt::SolidLine, Qt::FlatCap));
        int x = static_cast<int>(m_geometry.left() + fieldV + m_parComp.reaction * m_freq * step);
        int y1 = static_cast<int>(lineComp0 - (100 + m_parComp.statism + 3 * m_parComp.deviation) * propComp);
        int h = static_cast<int>(2 * 3 *m_parComp.deviation * propComp);
        m_painter->drawRect(x, y1, m_geometry.left() + m_geometry.width() - x, h);
        x = static_cast<int>(m_geometry.left() + fieldV + m_parRet.reaction * m_freq * step);
        y1 = static_cast<int>(lineRet0 - (100 + m_parRet.statism + 3 * m_parRet.deviation) * propRet);
        h = static_cast<int>(2 * 3 * m_parRet.deviation * propRet);
        m_painter->drawRect(x, y1, m_geometry.left() + m_geometry.width() - x, h);
        m_painter->setBrush(QBrush(backColor, Qt::SolidPattern));

        //! Оси
        m_painter->setPen(QPen(m_axisColor, 1, Qt::SolidLine, Qt::FlatCap));
        m_painter->drawLine(m_geometry.left() + 4, lineRetY, m_geometry.left() + m_geometry.width(), lineRetY);
        m_painter->drawLine(m_geometry.left() + 4, lineCompY, m_geometry.left() + m_geometry.width(), lineCompY);
        m_painter->drawLine(m_geometry.left() + fieldV, lineRetY, m_geometry.left() + fieldV, m_geometry.top());

        //! Линии 0% и 100%
        m_painter->setPen(QPen(m_axisColor, 1, Qt::DashLine, Qt::FlatCap));
        m_painter->drawLine(m_geometry.left() + fieldV, lineComp0, m_geometry.left() + m_geometry.width(), lineComp0);
        m_painter->drawLine(m_geometry.left() + fieldV, lineComp100, m_geometry.left() + m_geometry.width(), lineComp100);
        m_painter->drawText(m_geometry.left() + 6, lineComp0 + 4, "0%");
        m_painter->drawText(m_geometry.left() + 1, lineComp100 + 4, "100%");

        m_painter->drawLine(m_geometry.left() + fieldV, lineRet0, m_geometry.left() + m_geometry.width(), lineRet0);
        m_painter->drawLine(m_geometry.left() + fieldV, lineRet100, m_geometry.left() + m_geometry.width(), lineRet100);
        m_painter->drawText(m_geometry.left() + 6, lineRet0 + 4, "0%");
        m_painter->drawText(m_geometry.left() + 1, lineRet100 + 4, "100%");

        //! Графики
        for (int i = 0; i < m_compensation.size() - 1; ++i)
        {
            int x1 = static_cast<int>(m_geometry.left() + fieldV + i * step);
            int x2 = static_cast<int>(m_geometry.left() + fieldV + (i + 1) * step);

            //! Компенсация
            m_painter->setPen(QPen(getColorTransient(i, m_parComp), 2 * ratio, Qt::SolidLine, Qt::FlatCap));
            int y1 = static_cast<int>(lineComp0 - m_compensation[i] * propComp);
            int y2 = static_cast<int>(lineComp0 - m_compensation[i + 1] * propComp);
            m_painter->drawLine(x1, y1, x2, y2);

            //! Возврат
            m_painter->setPen(QPen(getColorTransient(i, m_parRet), 2 * ratio, Qt::SolidLine, Qt::FlatCap));
            y1 = static_cast<int>(lineRet0 - m_return[i] * propRet);
            y2 = static_cast<int>(lineRet0 - m_return[i + 1] * propRet);
            m_painter->drawLine(x1, y1, x2, y2);

            //! Секундные метки
            if ((i % m_freq) == 0)
            {
                m_painter->setPen(QPen(m_axisColor, 1, Qt::DotLine, Qt::FlatCap));
                m_painter->drawLine(x1, m_geometry.top(), x1, lineRetY);
                QString s = QString::number(i / m_freq);
                m_painter->drawText(x1 - 4, m_geometry.top() + m_geometry.height() - 5, s);
            }
        }

        //! Метки латентного периода
        m_painter->setPen(QPen(m_latentColor, 1, Qt::DashLine, Qt::FlatCap));
        x = static_cast<int>(m_geometry.left() + fieldV + m_parComp.latent * m_freq * step);
        m_painter->drawLine(x, m_geometry.top(), x, lineCompY);
        x = static_cast<int>(m_geometry.left() + fieldV + m_parRet.latent * m_freq * step);
        m_painter->drawLine(x, lineCompY, x, lineRetY);

        //! Метки размаха
        m_painter->setPen(QPen(m_swingColor, 1, Qt::DashLine, Qt::FlatCap));
        x = static_cast<int>(m_geometry.left() + fieldV + m_parComp.swing * m_freq * step);
        m_painter->drawLine(x, m_geometry.top(), x, lineCompY);
        x = static_cast<int>(m_geometry.left() + fieldV + m_parRet.swing * m_freq * step);
        m_painter->drawLine(x, lineCompY, x, lineRetY);

        //! Метки броска
        m_painter->setPen(QPen(m_spurtColor, 1, Qt::DashLine, Qt::FlatCap));
        x = static_cast<int>(m_geometry.left() + fieldV + m_parComp.spurt * m_freq * step);
        m_painter->drawLine(x, m_geometry.top(), x, lineCompY);
        x = static_cast<int>(m_geometry.left() + fieldV + m_parRet.spurt * m_freq * step);
        m_painter->drawLine(x, lineCompY, x, lineRetY);

        //! Метки времени реакции
        m_painter->setPen(QPen(m_stabilisationColor, 1, Qt::DashLine, Qt::FlatCap));
        x = static_cast<int>(m_geometry.left() + fieldV + m_parComp.reaction * m_freq * step);
        m_painter->drawLine(x, m_geometry.top(), x, lineCompY);
        x = static_cast<int>(m_geometry.left() + fieldV + m_parRet.reaction * m_freq * step);
        m_painter->drawLine(x, lineCompY, x, lineRetY);
    }

    m_painter->restore();
}

void TransientsPainter::doUpdate()
{

}

QColor TransientsPainter::getColorTransient(const int cnt, const TransientsPainter::Params &params) const
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
