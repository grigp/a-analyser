#include "tetrisglass.h"

#include "baseutils.h"

#include <QPainter>
#include <QDebug>

TetrisGlass::TetrisGlass(QGraphicsItem *parent)
    : QGraphicsItem (parent)
{

}

QRectF TetrisGlass::boundingRect() const
{
    return QRectF(m_left, m_top, m_width, m_height);
}

void TetrisGlass::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    //! Фон стакана
    if (m_style == stlPlate)
    {
        QRect rect(static_cast<int>(boundingRect().x()), static_cast<int>(boundingRect().y()),
                   static_cast<int>(boundingRect().width()), static_cast<int>(boundingRect().height()));

        int x = static_cast<int>(boundingRect().x());
        while (x < boundingRect().x() + boundingRect().width())
        {
            int y = static_cast<int>(boundingRect().y());
            while(y < boundingRect().y() + boundingRect().height())
            {
                //! Если изображение выходит за границы, его надо обрезать
                auto d = x + m_pixmapPlite.width() - (boundingRect().x() + boundingRect().width());
                if (d > 0)
                    painter->drawPixmap(x, y, m_pixmapPlite, 0, 0, static_cast<int>(m_pixmapPlite.width() - d), m_pixmapPlite.height());
                else
                    painter->drawPixmap(x, y, m_pixmapPlite);
                y += m_pixmapPlite.height();
            }
            x += m_pixmapPlite.width();
        }
    }
    else
    if (m_style == stlColor)
    {
        painter->setBrush(QBrush(m_color, Qt::SolidPattern));
        painter->setPen(QPen(m_color, 1, Qt::SolidLine, Qt::FlatCap));
        painter->drawRect(boundingRect());
    }

    //! Рамка стакана
    int y = static_cast<int>(boundingRect().y());
    while(y < boundingRect().y() + boundingRect().height() - m_pixmapFrmLeft.height())
    {
        painter->drawPixmap(static_cast<int>(boundingRect().x()), y, m_pixmapFrmLeft);
        painter->drawPixmap(static_cast<int>(boundingRect().x() + boundingRect().width() - m_pixmapFrmRight.width()), y, m_pixmapFrmRight);
        y += m_pixmapFrmLeft.height();
    }
    int x = static_cast<int>(boundingRect().x());
    while (x < boundingRect().x() + boundingRect().width() - m_pixmapFrmBottom.width())
    {
        painter->drawPixmap(x, static_cast<int>(boundingRect().y() + boundingRect().height() - m_pixmapFrmBottom.height()), m_pixmapFrmBottom);
        x += m_pixmapFrmBottom.width();
    }
    //! Уголки рамки
    painter->drawPixmap(static_cast<int>(boundingRect().x()),
                        static_cast<int>(boundingRect().y() + boundingRect().height() - m_pixmapFrmCornerLeft.height()),
                        m_pixmapFrmCornerLeft);
    painter->drawPixmap(static_cast<int>(boundingRect().x() + boundingRect().width() - m_pixmapFrmCornerRight.width()),
                        static_cast<int>(boundingRect().y() + boundingRect().height() - m_pixmapFrmCornerRight.height()),
                        m_pixmapFrmCornerRight);

    //! Заполненные кубики
    {
        qreal y = boundingRect().y() + boundingRect().height() - m_pixmapFrmBottom.height() - m_cubeSize;
        for (int v = 0; v < m_data.size(); ++v)
        {
            qreal x = boundingRect().x() + m_pixmapFrmLeft.width();
            for (int h = 0; h < m_data[v].size(); ++h)
            {
                if (m_data[v][h] != Qt::black)
                {
                    QRectF source(0, 0, m_pixmapCube.width(), m_pixmapCube.height());
                    QRectF target(x, y, m_cubeSize, m_cubeSize);
                    painter->drawPixmap(target, m_pixmapCube, source);
                }
                x += m_cubeSize;
            }

            y -= m_cubeSize;
        }
    }

//    painter->setBrush(QBrush(Qt::red, Qt::NoBrush));
//    painter->setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::FlatCap));
//    painter->drawRect(boundingRect());
}

void TetrisGlass::setGeometry(const qreal sceneWidth, const qreal sceneHeight, const int h, const int v)
{
    m_height = sceneHeight;
    m_cubeSize = (sceneHeight - m_glassBorderB) / v;
    m_width = (m_cubeSize * h) + m_glassBorderLR * 2;
    m_top = 0;
    m_left = sceneWidth / 2 - m_width / 2;
    m_hCount = h;
    m_vCount = v;
    fillData();
}

void TetrisGlass::setPliteImage(const QString fn)
{
    m_pixmapPlite.load(":/images/plite_textures/" + fn);
}

void TetrisGlass::setFrameLeftImage(const QString fn)
{
    m_pixmapFrmLeft.load(":/images/Frames/" + fn);
    BaseUtils::setColoredPicture(m_pixmapFrmLeft, m_frameColor);
    m_glassBorderLR = m_pixmapFrmLeft.width();
}

void TetrisGlass::setFrameRightImage(const QString fn)
{
    m_pixmapFrmRight.load(":/images/Frames/" + fn);
    BaseUtils::setColoredPicture(m_pixmapFrmRight, m_frameColor);
    m_glassBorderLR = m_pixmapFrmRight.width();
}

void TetrisGlass::setFrameBottomImage(const QString fn)
{
    m_pixmapFrmBottom.load(":/images/Frames/" + fn);
    BaseUtils::setColoredPicture(m_pixmapFrmBottom, m_frameColor);
    m_glassBorderB = m_pixmapFrmBottom.height();
}

void TetrisGlass::setFrameCornerLeftImage(const QString fn)
{
    m_pixmapFrmCornerLeft.load(":/images/Frames/" + fn);
    BaseUtils::setColoredPicture(m_pixmapFrmCornerLeft, m_frameColor);
}

void TetrisGlass::setFrameCornerRightImage(const QString fn)
{
    m_pixmapFrmCornerRight.load(":/images/Frames/" + fn);
    BaseUtils::setColoredPicture(m_pixmapFrmCornerRight, m_frameColor);
}

void TetrisGlass::setCubeImage(const QString fn)
{
    m_pixmapCube.load(":/images/Games/" + fn);
}

QColor TetrisGlass::value(const int h, const int v) const
{
    Q_ASSERT(v >= 0 && v < m_data.size());
    Q_ASSERT(h >= 0 && h < m_data[v].size());
    return m_data[v][h];
}

void TetrisGlass::setValue(const int h, const int v, const QColor value)
{
    Q_ASSERT(v >= 0 && v < m_data.size());
    Q_ASSERT(h >= 0 && h < m_data[v].size());
    m_data[v][h] = value;
}

void TetrisGlass::fillData()
{
    m_data.resize(m_vCount);
    for (int v = 0; v < m_data.size(); ++v)
    {
        m_data[v].resize(m_hCount);
        for (int h = 0; h < m_data[v].size(); ++h)
            m_data[v][h] = QColor(Qt::black);

    }
//    foreach (auto row, m_data)
//    {
//        row.resize(m_hCount);
//        foreach (auto rc, row)
//            rc = QColor(Qt::black);
//    }
}


