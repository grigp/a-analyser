#include "octaedronpainter.h"

#include <QDebug>

namespace
{
struct ArrowIndexes
{
    int from;
    int to;
    ArrowIndexes(const int f, const int t)
        : from(f), to(t) {}
};

//! Порядок индексов для прохода в кольцевом режиме по часовой стрелке
static const QList<ArrowIndexes> CircleCWIndexes =
{
    ArrowIndexes(0, 1),
    ArrowIndexes(1, 2),
    ArrowIndexes(2, 3),
    ArrowIndexes(3, 4),
    ArrowIndexes(4, 5),
    ArrowIndexes(5, 6),
    ArrowIndexes(6, 7),
    ArrowIndexes(7, 0)
};

//! Порядок индексов для прохода в кольцевом режиме против часовой стрелки
static const QList<ArrowIndexes> CircleCCWIndexes =
{
    ArrowIndexes(0, 7),
    ArrowIndexes(1, 0),
    ArrowIndexes(2, 1),
    ArrowIndexes(3, 2),
    ArrowIndexes(4, 3),
    ArrowIndexes(5, 4),
    ArrowIndexes(6, 5),
    ArrowIndexes(7, 6)
};

}


OctaedronPainter::OctaedronPainter(QPainter *painter, QRect geometry)
    : m_painter(painter), m_geometry(geometry)
{

}

OctaedronPainter::OctaedronPainter()
{

}

OctaedronPainter::~OctaedronPainter()
{

}

void OctaedronPainter::setCanvas(QPainter *painter, QRect geometry, QWidget *wgt)
{
    m_painter = painter;
    m_geometry = geometry;
    m_widget = wgt;
}

void OctaedronPainter::setData(const int idx, const int value)
{
    Q_ASSERT(idx >= 0 && idx < 8);
    Q_ASSERT(value >= 0 && value <= 100);
    m_data[idx] = value;
}

int OctaedronPainter::data(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < 8);
    return m_data[idx];
}

void OctaedronPainter::doPaint(const double ratio)
{
    m_painter->save();

    QColor backColor = Qt::white;
    if (m_widget)
        backColor = m_widget->palette().background().color();

    m_painter->setBrush(QBrush(backColor, Qt::SolidPattern));
    m_painter->setPen(QPen(backColor, 1, Qt::SolidLine, Qt::FlatCap));
    m_painter->drawRect(m_geometry);

    int mx = m_geometry.center().x();
    int my = m_geometry.center().y();
    int radius = static_cast<int>(m_geometry.height() / 2 * 0.85);
    const int sizeLabel = static_cast<int>(40 * ratio);

    int tw = 18;
    if (ratio > 1)
        tw = static_cast<int>(18 / (ratio / 2));
    m_painter->setFont(QFont("Arial", tw, 0, false));

    //! Прямоугольники меток
    QList<QRect> labels = {
        QRect(mx - sizeLabel / 2, my - radius - sizeLabel / 2, sizeLabel, sizeLabel),
        QRect(static_cast<int>(mx - radius * cos(M_PI_4) - sizeLabel / 2),
              static_cast<int>(my - radius * sin(M_PI_4) - sizeLabel / 2), sizeLabel, sizeLabel),
        QRect(mx - radius - sizeLabel / 2, my - sizeLabel / 2, sizeLabel, sizeLabel),
        QRect(static_cast<int>(mx - radius * 0.8 * cos(M_PI_4) - sizeLabel / 2),
              static_cast<int>(my + radius * 0.8 * sin(M_PI_4) - sizeLabel / 2), sizeLabel, sizeLabel),
        QRect(mx - sizeLabel / 2, static_cast<int>(my + radius * 0.7 - sizeLabel / 2), sizeLabel, sizeLabel),
        QRect(static_cast<int>(mx + radius * 0.8 * cos(M_PI_4) - sizeLabel / 2),
              static_cast<int>(my + radius * 0.8 * sin(M_PI_4) - sizeLabel / 2), sizeLabel, sizeLabel),
        QRect(mx + radius - sizeLabel / 2, my - sizeLabel / 2, sizeLabel, sizeLabel),
        QRect(static_cast<int>(mx + radius * cos(M_PI_4) - sizeLabel / 2),
              static_cast<int>(my - radius * sin(M_PI_4) - sizeLabel / 2), sizeLabel, sizeLabel)
    };

    //! Диаграмма
    m_painter->setBrush(QBrush(m_labelColor, Qt::SolidPattern));
    m_painter->setPen(QPen(m_lineColor, 5 * ratio, Qt::SolidLine, Qt::FlatCap));

    //! Радиальный режим
    if (m_mode == 0)
    {
        for (int i = 0; i < labels.size(); ++i)
        {
            int lx = labels.at(i).center().x();
            int ly = labels.at(i).center().y();
            m_painter->drawLine(mx,
                                my,
                                mx + (lx - mx) * m_data[i] / 100,
                                my + (ly - my) * m_data[i] / 100);
        }
    }
    else
    //! Кольцевой режим
    if (m_mode == 1)
    {
        QList<ArrowIndexes> indexes = CircleCWIndexes;
        if (m_direction == 2)
            indexes = CircleCCWIndexes;
        for (int i = 0; i < labels.size(); ++i)
        {
            int lx = labels.at(indexes.at(i).to).center().x();
            int ly = labels.at(indexes.at(i).to).center().y();
            int tx = labels.at(indexes.at(i).from).center().x();
            int ty = labels.at(indexes.at(i).from).center().y();
            m_painter->drawLine(lx, ly, lx + (tx - lx) * m_data[i] / 100, ly + (ty - ly) * m_data[i] / 100);
        }
    }

    //! Метки
    for (int i = 0; i < labels.size(); ++i)
    {
        m_painter->setPen(QPen(m_labelFrameColor, 1, Qt::SolidLine, Qt::FlatCap));
        m_painter->drawRect(labels.at(i));

        m_painter->setPen(QPen(m_labelTextColor, 1, Qt::SolidLine, Qt::FlatCap));

        QString sLabel = "";
        if (m_lm == lmValue)
            sLabel = QString::number(m_data[i]);
        else
        if (m_lm == lmIndex)
            sLabel = QString::number(i);
        else
        if (m_lm == lmIndexPlus1)
            sLabel = QString::number(i+1);

        m_painter->drawText(labels.at(i), sLabel, QTextOption(Qt::AlignHCenter | Qt::AlignVCenter));
    }

    m_painter->restore();
}

void OctaedronPainter::doUpdate()
{

}
