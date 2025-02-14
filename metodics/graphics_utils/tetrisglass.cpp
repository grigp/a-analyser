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
                    QColor color = m_data[v][h];
                    if (m_deletingCubes.contains(QPoint(h, v)))
                        color = m_deletingCubesColor;

                    int colorCode = color.red() * 16777216 + color.green() * 65536 + color.blue() * 256 + color.alpha();
                    if (m_allowColors.contains(colorCode))
                    {
                        painter->drawPixmap(static_cast<int>(x), static_cast<int>(y), m_allowColors.value(colorCode));
                    }
                }


                //! Сетка
                if (m_isShowGrid)
                {
                    painter->setPen(QPen(Qt::darkGray, 1, Qt::DotLine, Qt::FlatCap));
                    painter->drawLine(static_cast<int>(x), 0,
                                      static_cast<int>(x), static_cast<int>(boundingRect().y() + boundingRect().height() - m_pixmapFrmBottom.height()));
                    painter->drawText(static_cast<int>(x + m_cubeSize / 2),
                                      static_cast<int>(boundingRect().y() + boundingRect().height() - m_pixmapFrmBottom.height() - 30),
                                      QString::number(h));
                }

                x += m_cubeSize;
            }

            //! Сетка
            if (m_isShowGrid)
            {
                painter->setPen(QPen(Qt::darkGray, 1, Qt::DotLine, Qt::FlatCap));
                painter->drawLine(static_cast<int>(boundingRect().x()), static_cast<int>(y),
                                  static_cast<int>(boundingRect().x() + boundingRect().width()), static_cast<int>(y));
                if (v != 0)
                {
                    painter->drawText(static_cast<int>(boundingRect().x() + m_pixmapFrmLeft.width() + m_cubeSize / 2),
                                      static_cast<int>(y + m_cubeSize / 2),
                                      QString::number(v));
                    painter->drawText(static_cast<int>(boundingRect().x() + boundingRect().width() - m_pixmapFrmRight.width() - m_cubeSize / 2),
                                      static_cast<int>(y + m_cubeSize / 2),
                                      QString::number(v));
                }
            }

            y -= m_cubeSize;
        }
    }

    //! Фигура
    showFigure(painter);

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
    m_pixmapCube.load(fn);
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

void TetrisGlass::addColor(const QColor color)
{
    int colorCode = color.red() * 16777216 + color.green() * 65536 + color.blue() * 256 + color.alpha();
    QPixmap cube = m_pixmapCube.scaled(static_cast<int>(m_cubeSize), static_cast<int>(m_cubeSize)); //, Qt::KeepAspectRatio);
    BaseUtils::setColoredPicture(cube, color);
    m_allowColors.insert(colorCode, cube);
}

void TetrisGlass::setNewFigure(QVector<QVector<QColor>> figure)
{
    m_figure = figure;
    int size = qMax(m_figure.size(), m_figure[0].size());
    m_figX = boundingRect().x() + boundingRect().width() / 2;
    m_figY = boundingRect().y() + m_cubeSize * size / 2;
    m_figAngleCode = 0;
}

void TetrisGlass::rotateFigure()
{
    ++m_figAngleCode;
}

QRectF TetrisGlass::getFigureRect() const
{
    int iSize = m_figure.size();
    int jSize = m_figure[0].size();
    if (m_figAngleCode % 4 == 0 || m_figAngleCode % 4 == 2)
        return QRectF(m_figX - jSize * m_cubeSize / 2, m_figY - iSize * m_cubeSize / 2, jSize * m_cubeSize, iSize * m_cubeSize);
    else
    if (m_figAngleCode % 4 == 1 || m_figAngleCode % 4 == 3)
        return QRectF(m_figX - iSize * m_cubeSize / 2, m_figY - jSize * m_cubeSize / 2, iSize * m_cubeSize, jSize * m_cubeSize);

    return QRectF(0, 0, 0, 0);
}

QRect TetrisGlass::getFigurePosition() const
{
    int xMin = INT_MAX;
    int xMax = -INT_MAX;
    int yMin = INT_MAX;
    int yMax = -INT_MAX;

    auto p = getFigureRect();                 //! Позиция и размер фигуры

    auto x = p.x() + m_cubeSize / 2;
    while (x < p.x() + p.width())
    {
        auto y = p.y() + m_cubeSize / 2;
        while (y < p.y() + p.height())
        {
            auto pos = getPositionByCoord(x, y);  //! Позицию для кубика в фигуре

            if (pos.x() < xMin) xMin = pos.x();
            if (pos.x() > xMax) xMax = pos.x();
            if (pos.y() < yMin) yMin = pos.y();
            if (pos.y() > yMax) yMax = pos.y();

            y += m_cubeSize;
        }

        x += m_cubeSize;
    }

    return QRect(xMin, yMin, xMax - xMin + 1, yMax - yMin + 1);
}

QColor TetrisGlass::getFigureColor(const int h, const int v)
{
    if (m_figAngleCode % 4 == 0 || m_figAngleCode % 4 == 2)
    {
        Q_ASSERT(v >= 0 && v < m_figure.size());
        Q_ASSERT(h >= 0 && h < m_figure[0].size());
        if (m_figAngleCode % 4 == 0)
            return  m_figure[v][h];
        else
        if (m_figAngleCode % 4 == 2)
            return  m_figure[m_figure.size() - 1 - v][m_figure[0].size() - 1 - h];
    }
    else
    if (m_figAngleCode % 4 == 1 || m_figAngleCode % 4 == 3)
    {
        Q_ASSERT(h >= 0 && h < m_figure.size());
        Q_ASSERT(v >= 0 && v < m_figure[0].size());
        if (m_figAngleCode % 4 == 1)
            return  m_figure[m_figure.size() - 1 - h][v];
        else
        if (m_figAngleCode % 4 == 3)
            return  m_figure[h][m_figure[0].size() - 1 - v];
    }
    return Qt::black;
}

//bool TetrisGlass::setFigureCoordinates(const qreal x, const qreal y)
//{
//    m_figX = x;
//    m_figY = y;
//    auto pos = getFigurePosition();
//    qreal w = pos.width();
//    qreal h = pos.height();
////    if (x >= 0 && x < m_hCount)
////    {


////    }

////    m_figX = boundingRect().x() + m_glassBorderLR + pos.x() * m_cubeSize + w / 2;
////    m_figY = boundingRect().y() + (m_vCount - pos.y() - 1) * m_cubeSize - h / 2;
//    qDebug() << x << y << pos << "  " << m_figX << m_figY;

//    return false;
//}

bool TetrisGlass::setFigureCoordinates(const qreal x, const qreal y)
{
    auto pos = getFigurePosition();
    qreal left = 0;
    qreal right = 0;
    qreal up = 0;
    qreal down = 0;
    getAvaibleDiapHorizontal(pos, left, right);
    getAvaibleDiapVertical(pos, up, down);

    auto correctByAvaibleDiap = [&]()
    {
        if (m_figX - static_cast<double>(pos.width()) / 2 * m_cubeSize < left)
            m_figX = left + static_cast<double>(pos.width()) / 2 * m_cubeSize;
        if (m_figX + static_cast<double>(pos.width()) / 2 * m_cubeSize > right)
            m_figX = right - static_cast<double>(pos.width()) / 2 * m_cubeSize;
        if (m_figY + static_cast<double>(pos.height()) / 2 * m_cubeSize > down)
            m_figY = down - static_cast<double>(pos.height()) / 2 * m_cubeSize;
        if (m_figY - static_cast<double>(pos.height()) / 2 * m_cubeSize < up)
            m_figY = up + static_cast<double>(pos.height()) / 2 * m_cubeSize;
    };

    if (x >= 0 && (x + pos.width() * m_cubeSize) < boundingRect().x() + boundingRect().width())
    {
        //! Смещаем фигуру от точки (m_figX, m_figY) к точке (x, y) по шагам.
        //! Размер шага - +-1 по большей длине изменения между фронталью и сагитталью
        int stepCnt = static_cast<int>(fmax(fabs(x - m_figX), fabs(y - m_figY)));  //! Кол-во шагов

        if (stepCnt > 0)
        {
            qreal dx = (x - m_figX) / stepCnt;  //! Инкремент на каждом шаге
            qreal dy = (y - m_figY) / stepCnt;
            //! Цикл по шагам
            for (int i = 0; i < stepCnt; ++i)
            {
                //! Смещаем фигуру
                if (((dx / fabs(dx) < 0) && (m_figX > x)) ||  //! dx < 0 и m_figX не достигло конечной точки сверху
                    ((dx / fabs(dx) > 0) && (m_figX < x)))    //! dx > 0 и m_figX не достигло конечной точки снизу
                    m_figX += dx;                             //! Только тогда инкремент!
                if (((dy / fabs(dy) < 0) && (m_figY > y)) ||  //! dy < 0 и m_figY не достигло конечной точки сверху
                    ((dy / fabs(dy) > 0) && (m_figY < y)))    //! dy > 0 и m_figY не достигло конечной точки снизу
                    m_figY += dy;

                //! Коррекция позиции
//                auto bx = m_figX;

                correctByAvaibleDiap();

                //correctFigurePosition(m_figX, m_figY);

                //! Если в процессе поставили на целевую позицию, то выходим и сообщаем, что надо укладывать
                if (!figureIntoConfig() && figureOnConfigOrBottom())
                    return true;
            }
        }
    }

    //! Фигуру на конечную позицию
    m_figX = x;
    m_figY = y;

    correctByAvaibleDiap();

    //! Если поставили на целевую позицию, то выходим и сообщаем, что надо укладывать
    //correctFigurePosition(m_figX, m_figY);

    return !figureIntoConfig() && figureOnConfigOrBottom();
}

QPoint TetrisGlass::getPositionByCoord(const qreal x, const qreal y) const
{
    int px = static_cast<int>((x - boundingRect().left() - m_glassBorderLR) / m_cubeSize);
    if (px < 0)
        px = 0;
    if (px >= m_hCount)
        px = m_hCount - 1;
    int py = static_cast<int>((boundingRect().top() + boundingRect().height() - m_glassBorderB - y) / m_cubeSize);
    if (py >= m_vCount)
        py = m_vCount - 1;
    return QPoint(px, py);
}

QRectF TetrisGlass::getCoordinatesOfPosition(const QPoint pos) const
{
//    Q_ASSERT(pos.x() >= 0 && pos.x() < m_hCount);
//    Q_ASSERT(pos.y() >= 0 && pos.y() < m_vCount);

    qreal left = boundingRect().left() + m_glassBorderLR + pos.x() * m_cubeSize;
    qreal top = boundingRect().top() + boundingRect().height() - m_glassBorderB - pos.y() * m_cubeSize;
    return QRectF(left, top, m_cubeSize, m_cubeSize);
}

void TetrisGlass::setDeletingCube(const int h, const int v)
{
    m_deletingCubes.append(QPoint(h, v));
}

void TetrisGlass::clearDeletingCubes()
{
    m_deletingCubes.clear();
}

void TetrisGlass::setDeletingCubesColor(const QColor color)
{
    m_deletingCubesColor = color;
}

void TetrisGlass::clear()
{
    for (int i = 0; i < m_vCount; ++i)
        for (int j = 0; j < m_hCount; ++j)
        {
            m_data[i][j] = Qt::black;
        }
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
}

void TetrisGlass::showFigure(QPainter *painter)
{
    int figAngle = m_figAngleCode % 4;  //! В диапазон 0 - 4
    int iSize = m_figure.size();
    for (int i = 0; i < iSize; ++i)
    {
        int jSize = m_figure[i].size();
        for (int j = 0; j < jSize; ++j)
        {
            QColor color = m_figure[i][j];
            if (color != Qt::black)
            {
                int colorCode = color.red() * 16777216 + color.green() * 65536 + color.blue() * 256 + color.alpha();
                if (m_allowColors.contains(colorCode))
                {
                    if (figAngle == 0)
                        painter->drawPixmap(static_cast<int>(m_figX - jSize * m_cubeSize / 2 + j * m_cubeSize),
                                            static_cast<int>(m_figY - iSize * m_cubeSize / 2 + i * m_cubeSize),
                                            m_allowColors.value(colorCode));
                    else
                    if (figAngle == 1)
                        painter->drawPixmap(static_cast<int>(m_figX - iSize * m_cubeSize / 2 + (iSize - i - 1) * m_cubeSize),
                                            static_cast<int>(m_figY - jSize * m_cubeSize / 2 + j * m_cubeSize),
                                            m_allowColors.value(colorCode));
                    else
                    if (figAngle == 2)
                        painter->drawPixmap(static_cast<int>(m_figX - jSize * m_cubeSize / 2 + (jSize - j - 1) * m_cubeSize),
                                            static_cast<int>(m_figY - iSize * m_cubeSize / 2 + (iSize - i - 1) * m_cubeSize),
                                            m_allowColors.value(colorCode));
                    else
                    if (figAngle == 3)
                        painter->drawPixmap(static_cast<int>(m_figX - iSize * m_cubeSize / 2 + i * m_cubeSize),
                                            static_cast<int>(m_figY - jSize * m_cubeSize / 2 + (jSize - j - 1) * m_cubeSize),
                                            m_allowColors.value(colorCode));
                }
            }
        }
    }

    //! Точка позиционирования фигуры - служебное
    if (m_isShowFigurePos)
    {
        int n = 7;
        painter->setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::FlatCap));
        painter->drawLine(static_cast<int>(m_figX), static_cast<int>(m_figY - n), static_cast<int>(m_figX), static_cast<int>(m_figY + n));
        painter->drawLine(static_cast<int>(m_figX - n), static_cast<int>(m_figY), static_cast<int>(m_figX + n), static_cast<int>(m_figY));
    }
}

bool TetrisGlass::figureOnConfigOrBottom()
{
    auto p = getFigureRect();                 //! Позиция и размер фигуры

    auto x = p.x() + m_cubeSize / 2;
    while (x < p.x() + p.width())
    {
        auto y = p.y() + p.height() - m_cubeSize / 2;
        auto pos = getPositionByCoord(x, y);  //! Позицию для кубика в фигуре

        //! В рамках стакана
        if (pos.x() >= 0 && pos.x() < m_hCount)
        {
            //! Достигли дна или фигур снизу
            if (pos.y() == 0 || m_data[pos.y() - 1][pos.x()] != Qt::black)
                return true;
        }

        x += m_cubeSize;
    }

    return false;
}

bool TetrisGlass::figureIntoConfig()
{
    auto p = getFigureRect();                 //! Позиция и размер фигуры

    auto x = p.x() + m_cubeSize / 2;
    while (x < p.x() + p.width())
    {
        auto y = p.y() + m_cubeSize / 2;
        while (y < p.y() + p.height())
        {
            auto pos = getPositionByCoord(x, y);  //! Позицию для кубика в фигуре

            //! В рамках стакана
            if (pos.x() >= 0 && pos.x() < m_hCount && pos.y() >= 0)
            {
                //! Находимся внутри конфигурации - нужна корректировка
                if (m_data[pos.y()][pos.x()] != Qt::black)
                {
                    return true;
                }
            }

            y += m_cubeSize;
        }

        x += m_cubeSize;
    }

    return false;
}

void TetrisGlass::correctFigurePosition(const qreal tx, const qreal ty)
{
    Q_UNUSED(tx);
    Q_UNUSED(ty);
    auto correct = [&]()
    {
        auto p = getFigureRect();                 //! Позиция и размер фигуры
        qreal dx = m_figX - p.x();                          //! Смещение переданной позиции относительно верхнего левого угла
        qreal dy = m_figY - p.y();
        auto pos = getPositionByCoord(p.x(), p.y());  //! Позицию для левого верхнего угла фигуры

        if (pos.x() < 0)
            pos.setX(0);
        if (pos.x() > m_hCount - p.width() / m_cubeSize)
            pos.setX(static_cast<int>(m_hCount - p.width() / m_cubeSize));

        auto pos_c = getCoordinatesOfPosition(pos);   //! Координаты фиксированной позиции для левого верхнего угла фигуры
        m_figX = pos_c.x() + dx;                       //! Устанавливаем фигуру на фиксированную позицию + смещение
        m_figY = pos_c.y() + dy;
    };

    correct();

//    //! Фигура внутри конфигурации - надо корректировать
//    if (figureIntoConfig())
//    {
//        qreal dx = tx - m_figX;
//        qreal dy = ty - m_figY;

//        qDebug() << "----------" << m_figX << tx << "   " << m_figY << ty;
//        qDebug() << "----------" << dx << dy;
//        qreal vx = m_figX;
//        qreal vy = m_figY;

//        if (fabs(dx) >= fabs(dy))
//        {
//            qDebug() << vx << vy << fabs(vx - tx);
//            while (fabs(vx - tx) > 1)
//            {
//                vy += m_cubeSize * fabs(dy / dx) * (dy / fabs(dy));
//                vx += m_cubeSize * (dx / fabs(dx));   //! +-m_cubeSize

//                auto oFigX = m_figX;
//                auto oFigY = m_figY;
//                m_figX = vx;
//                m_figY = vy;

//               // correct();

//                bool foc = figureIntoConfig();
//                qDebug() << "x" << m_figX << m_figY << foc;

//                if (!foc)
//                {
//                    m_figX = oFigX;
//                    m_figY = oFigY;
//                    break;
//                }
//            }
//        }
//        else
//        {
//            qDebug() << vx << vy << fabs(vx - tx);
//            while (fabs(vy - ty) > 1)
//            {
//                vx += m_cubeSize * fabs(dx / dy) * (dx / fabs(dx));
//                vy += m_cubeSize * (dy / fabs(dy));   //! +-m_cubeSize

//                auto oFigX = m_figX;
//                auto oFigY = m_figY;
//                m_figX = vx;
//                m_figY = vy;

//               // correct();

//                bool foc = figureIntoConfig();
//                qDebug() << "y" << m_figX << m_figY << foc;

//                if (!foc)
//                {
//                    m_figX = oFigX;
//                    m_figY = oFigY;
//                    break;
//                }
//            }
//        }

//    }

}

void TetrisGlass::getAvaibleDiapHorizontal(const QRect pos, qreal &left, qreal &right)
{
    //! Границы в позициях
    left = -INT_MAX;
    right = INT_MAX;
    for (int i = pos.y(); i < pos.y() + pos.height(); ++i)
    {
        for (int j = pos.x() - 1; j >= 0; --j)
        {
            if (m_data[i][j] != Qt::black)
            {
                if (j > left)
                    left = j;
                break;
            }
        }

        for (int j = pos.x() + pos.width(); j < m_hCount; ++j)
            if (m_data[i][j] != Qt::black)
            {
                if (j < right)
                    right = j;
                break;
            }
    }

    //! Границы в координатах
    if (left >= 0)
        left = boundingRect().x() + m_glassBorderLR + (left + 1) * m_cubeSize;
    else
        left = boundingRect().x() + m_glassBorderLR;

    if (right < m_hCount)
        right = boundingRect().x() + m_glassBorderLR + right * m_cubeSize;
    else
        right = boundingRect().x() + boundingRect().width() - m_glassBorderLR;
}

void TetrisGlass::getAvaibleDiapVertical(const QRect pos, qreal &up, qreal &down)
{
    //! Границы в позициях
    up = INT_MAX;
    down = -INT_MAX;

    for (int i = pos.x(); i < pos.x() + pos.width(); ++i)
    {
        for (int j = pos.y(); j >= 0; --j)
        {
            if (m_data[j][i] != Qt::black)
            {
                if (j > down)
                    down = j;
                break;
            }
        }

        for (int j = pos.y() + pos.height(); j < m_vCount; ++j)
            if (m_data[j][i] != Qt::black)
            {
                if (j < up)
                    up = j;
                break;
            }
    }

    //! Границы в координатах
    if (down >= 0)
        down = boundingRect().y() + boundingRect().height() - m_glassBorderB - (down + 1) * m_cubeSize;
    else
        down = boundingRect().y() + boundingRect().height() - m_glassBorderB;

    if (up < m_vCount)
        up = boundingRect().y() + boundingRect().height() - m_glassBorderB - up * m_cubeSize;
    else
        up = boundingRect().y();
}


