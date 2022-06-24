#include "platforms.h"
#include "areaskgdefines.h"

#include <QPainter>
#include <QDebug>
#include <QWidget>

Platforms::Platforms(int diap, QGraphicsItem *parent)
    : QGraphicsItem (parent)
    , m_diap(diap)
{

}

QRectF Platforms::boundingRect() const
{
    QPointF pos(-m_diap * m_prop - AreaSKGDefines::I_LABEL_SPACE, -m_diap * m_prop - AreaSKGDefines::I_LABEL_SPACE);
    QSizeF size((m_diap * m_prop + AreaSKGDefines::I_LABEL_SPACE) * 2, (m_diap * m_prop + AreaSKGDefines::I_LABEL_SPACE) * 2);
    return QRectF(pos, size);
}

void Platforms::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    Q_UNUSED(item);
    // Параметры построения
    m_width = widget->size().width();
    m_height = widget->size().height();
    int minS = qMin(m_width, m_height);
    m_prop = static_cast<double>(minS / 2 - AreaSKGDefines::I_LABEL_SPACE) / static_cast<double>(m_diap);

    painter->save();

    painter->setBrush(QBrush(m_color, Qt::SolidPattern));
    painter->setPen(QPen(m_colorFrame, 2, Qt::SolidLine, Qt::FlatCap));
    painter->setFont(QFont("Sans", 20, QFont::Bold, false));

    for (int i = 0; i < m_platforms.size(); ++i)
    {
        QRect r = QRect(static_cast<int>(m_platforms.at(i).x() * m_prop),
                        static_cast<int>(-m_platforms.at(i).y() * m_prop),
                        static_cast<int>(m_platforms.at(i).width() * m_prop),
                        static_cast<int>(m_platforms.at(i).height() * m_prop));
        painter->drawRect(r);
        painter->drawText(r, QString::number(i + 1));
    }


    painter->restore();
}

void Platforms::setDiap(int diap)
{
    m_diap = diap;
}

void Platforms::addPlatform(QRect platform)
{
    m_platforms << platform;
}

void Platforms::setColorPlatform(QColor color)
{
    m_color = color;
    m_colorFrame =  QColor(m_color.red() / 8, m_color.green() / 8, m_color.blue() / 8);
}

void Platforms::setColorFrame(QColor color)
{
    m_colorFrame = color;
}
