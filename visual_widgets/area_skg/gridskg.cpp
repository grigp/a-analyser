#include "gridskg.h"
#include <QPainter>
#include <QDebug>
#include <QWidget>

static const int I_LABEL_SPACE = 20;

GridSKG::GridSKG(int diap, QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , m_diap(diap)
{

}

QRectF GridSKG::boundingRect() const
{
    QPointF pos(-m_diap * m_prop - I_LABEL_SPACE, -m_diap * m_prop - I_LABEL_SPACE);
    QSizeF size((m_diap * m_prop + I_LABEL_SPACE) * 2, (m_diap * m_prop + I_LABEL_SPACE) * 2);
    return QRectF(pos, size);
}

//int n = 0;

void GridSKG::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    Q_UNUSED(item);
    // Параметры построения
    m_width = widget->size().width();
    m_height = widget->size().height();
    int minS = qMin(m_width, m_height);
    m_prop = static_cast<double>(minS / 2 - I_LABEL_SPACE) / static_cast<double>(m_diap);
    int left = -m_diap * m_prop;
    int top = -m_diap * m_prop;
    int right = m_diap * m_prop;
    int bottom = m_diap * m_prop;

    painter->save();

    // Оси и метки 0
    painter->setPen(QPen(Qt::black, 1));
    painter->drawRect(left, top, right - left, bottom - top);
    painter->drawLine(0, top, 0, bottom);
    painter->drawLine(left, 0, right, 0);
    painter->drawText(QRectF(0-10, top - I_LABEL_SPACE, 20, I_LABEL_SPACE), Qt::AlignCenter, "0");
    painter->drawText(QRectF(0-10, bottom, 20, I_LABEL_SPACE), Qt::AlignCenter, "0");
    painter->rotate(90);
    painter->drawText(QRectF(0-10, top - I_LABEL_SPACE, 20, I_LABEL_SPACE), Qt::AlignCenter, "0");
    painter->drawText(QRectF(0-10, bottom, 20, I_LABEL_SPACE), Qt::AlignCenter, "0");
    painter->rotate(-90);

    // Сетка
    //qDebug() << ++n << m_diap << m_prop;
    for (int i = 1; i <= m_diap; ++i)
    {
        if (m_diap >= 30)
        {
            if (i % 50 == 0)
            {
                painter->setPen(QPen(Qt::darkGray, 1, Qt::DashLine));
                drawPositionGrid(painter, left, top, right, bottom, i, true);
            }
            else
            if (i % 10 == 0)
            {
                painter->setPen(QPen(Qt::darkGray, 1, Qt::DotLine));
                drawPositionGrid(painter, left, top, right, bottom, i, m_prop * 10 > 20);
            }
        }
        else
        {
            if (i % 5 == 0)
            {
                painter->setPen(QPen(Qt::darkGray, 1, Qt::DashLine));
                drawPositionGrid(painter, left, top, right, bottom, i, true);
            }
            else
            {
                painter->setPen(QPen(Qt::darkGray, 1, Qt::DotLine));
                drawPositionGrid(painter, left, top, right, bottom, i, m_prop > 20);
            }
        }
    }

    painter->restore();
}

void GridSKG::setDiap(int diap)
{
    //! Если до первой прорисовки сделать изменение масштаба, появятся коллизии
    if (m_width > 0 && m_height > 0)
    {
        m_diap = diap;
        int minS = qMin(m_width, m_height);
        m_prop = static_cast<double>(minS / 2 - I_LABEL_SPACE) / static_cast<double>(m_diap);
        update(boundingRect());
    }
}

int GridSKG::getLabelSpace()
{
    return I_LABEL_SPACE;
}

void GridSKG::drawPositionGrid(QPainter* painter, int left, int top, int, int bottom, int posGrid, bool isLabels)
{
    // Линии
    painter->drawLine(0 - posGrid * m_prop, top, 0 - posGrid * m_prop, bottom);
    painter->drawLine(0 + posGrid * m_prop, top, 0 + posGrid * m_prop, bottom);
    painter->drawLine(left, 0 - posGrid * m_prop, bottom, 0 - posGrid * m_prop);
    painter->drawLine(left, 0 + posGrid * m_prop, bottom, 0 + posGrid * m_prop);

    painter->setPen(Qt::black);

    if (isLabels)
    {
        // Текст горизонтальный
        painter->drawText(QRectF(0 - posGrid * m_prop-10, top - I_LABEL_SPACE, 20, I_LABEL_SPACE), Qt::AlignCenter, QString::number(-posGrid));
        painter->drawText(QRectF(0 - posGrid * m_prop-10, bottom, 20, I_LABEL_SPACE), Qt::AlignCenter, QString::number(-posGrid));
        painter->drawText(QRectF(0 + posGrid * m_prop-10, top - I_LABEL_SPACE, 20, I_LABEL_SPACE), Qt::AlignCenter, QString::number(posGrid));
        painter->drawText(QRectF(0 + posGrid * m_prop-10, bottom, 20, I_LABEL_SPACE), Qt::AlignCenter, QString::number(posGrid));

        // Текст вертикальный
        painter->rotate(-90);
        painter->drawText(QRectF(0 - posGrid * m_prop-10, top - I_LABEL_SPACE, 20, I_LABEL_SPACE), Qt::AlignCenter, QString::number(-posGrid));
        painter->drawText(QRectF(0 + posGrid * m_prop-10, top - I_LABEL_SPACE, 20, I_LABEL_SPACE), Qt::AlignCenter, QString::number(posGrid));
        painter->rotate(180);
        painter->drawText(QRectF(0 - posGrid * m_prop-10, top - I_LABEL_SPACE, 20, I_LABEL_SPACE), Qt::AlignCenter, QString::number(posGrid));
        painter->drawText(QRectF(0 + posGrid * m_prop-10, top - I_LABEL_SPACE, 20, I_LABEL_SPACE), Qt::AlignCenter, QString::number(-posGrid));
        painter->rotate(-90);
    }
}
