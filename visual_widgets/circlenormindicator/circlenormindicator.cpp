#include "circlenormindicator.h"
#include "ui_circlenormindicator.h"


#include <QDebug>
#include "math.h"

CircleNormIndicator::CircleNormIndicator(QWidget *parent) :
    QWidget(parent)
  , ui(new Ui::CircleNormIndicator)
  , m_lightColor(QColor(132, 189, 213))
  , m_shadowColor(QColor(68, 113, 128))
  , m_panelColor(QColor(63, 104, 118))
  , m_arrowColor(QColor(255, 0, 0))
  , m_topReflexUpColor(QColor(0xFF, 0xFF, 0xFF, 0xA0))
  , m_topReflexDownColor(QColor(0xFF, 0xFF, 0xFF, 0x00))
  , m_colorNormLight({
                     std::pair<int, QColor>(0, Qt::red)
                   , std::pair<int, QColor>(1, Qt::yellow)
                   , std::pair<int, QColor>(2, Qt::green)
    })
  , m_colorNormDark({
                    std::pair<int, QColor>(0, Qt::darkRed)
                  , std::pair<int, QColor>(1, Qt::darkYellow)
                  , std::pair<int, QColor>(2, Qt::darkGreen)
    })
{
    ui->setupUi(this);
}

CircleNormIndicator::~CircleNormIndicator()
{
    delete ui;
}

void CircleNormIndicator::setValue(int value)
{
    m_value = value;
    repaint();
}

int CircleNormIndicator::value()
{
    return m_value;
}

void CircleNormIndicator::setMin(const int min)
{
    m_minVal = min;
}

void CircleNormIndicator::setMax(const int max)
{
    m_maxVal = max;
}

void CircleNormIndicator::setGroupNorm(NormBounds &norm)
{
    m_isGroupNorm = true;
    m_groupNorms = norm;
}

void CircleNormIndicator::setPersonalNorm(NormBounds &norm)
{
    m_isPersonalNorm = true;
    m_personalNorms = norm;
}

void CircleNormIndicator::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_height = size().height();
    m_width = size().width();
    m_minDim = (m_height > m_width) ? m_width : m_height;
    m_half = m_minDim / 2;
    m_centerX = m_width / 2;
    m_centerY = m_height / 2;

    m_outerBorderWidth = m_minDim / 40;
    m_innerBorderWidth = m_minDim / 46;
    m_outerBorderRadius = m_half - m_outerBorderWidth;
    m_innerBorderRadius = m_half - (m_outerBorderWidth + m_innerBorderWidth);

    m_topReflexY = m_centerY + m_half / 2
            - (m_half / 2) / 2;
    m_bottomReflexY = m_centerY + m_half / 2
            + (m_half / 2) / 2;
    m_topReflexHeight = m_half / 3 / 5;
    m_topReflexWidth = m_half / 3 / 3;
//    bottomReflexHeight = half / 5;
//    bottomReflexWidth = half / 3;

    m_drawingRect.setTop((m_height - m_minDim) / 2);
    m_drawingRect.setLeft((m_width - m_minDim) / 2);
    m_drawingRect.setHeight(m_minDim);
    m_drawingRect.setWidth(m_minDim);
}

void CircleNormIndicator::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    draw();
}

void CircleNormIndicator::draw()
{
    QPainter p(this);

    QPen pen;
    pen.setStyle(Qt::NoPen);
    p.setPen(pen);

    //! Внешнее кольцо (с наклоном наружу, часть выпуклого ободка)
    QRadialGradient outerRingGradient(QPoint(m_centerX,
                                             m_centerY - m_outerBorderRadius - (m_outerBorderWidth / 2)),
                                      m_minDim - (m_outerBorderWidth / 2));
    outerRingGradient.setColorAt(0, m_lightColor);
    outerRingGradient.setColorAt(1, m_shadowColor);
    QBrush outerRingBrush(outerRingGradient);
    p.setBrush(outerRingBrush);
    p.drawEllipse(m_drawingRect);

    //! Внутреннее кольцо (с наклоном внутрь)
    QRadialGradient innerRingGradient(QPoint(m_centerX,
                                             m_centerY + m_innerBorderRadius + (m_innerBorderWidth / 2)),
                                      m_minDim - (m_innerBorderWidth / 2));
    innerRingGradient.setColorAt(0, m_lightColor);
    innerRingGradient.setColorAt(1, m_shadowColor);
    QBrush innerRingBrush(innerRingGradient);
    p.setBrush(innerRingBrush);
    p.drawEllipse(QPoint(m_centerX, m_centerY), m_outerBorderRadius, m_outerBorderRadius);

    //! Корпус стрелочного прибора
    QColor dark(m_panelColor.darker(120));
    QRadialGradient glassGradient(QPoint(m_centerX, m_centerY), m_innerBorderRadius);
    glassGradient.setColorAt(0, m_panelColor);
    glassGradient.setColorAt(1, dark);
    QBrush glassBrush(glassGradient);
    p.setBrush(glassBrush);
    p.drawEllipse(QPoint(m_centerX, m_centerY), m_innerBorderRadius, m_innerBorderRadius);

    //! Зона групповой нормы
    if (m_isGroupNorm)
        drawNorm(nkGroup, p);
    //! Зона индивидуальной нормы
    if (m_isPersonalNorm)
        drawNorm(nkPersonal, p);

    p.setPen(pen);

    //! Внутренняя часть стрелочного прибора для закраски секторов норм
    QColor darkIn(m_panelColor.darker(120));
    QRadialGradient glassGradientIn(QPoint(m_centerX, m_centerY), m_innerBorderRadius );
    glassGradientIn.setColorAt(0, m_panelColor);
    glassGradientIn.setColorAt(1, darkIn);
    QBrush glassBrushIn(glassGradientIn);
    p.setBrush(glassBrushIn);
    p.drawEllipse(QPoint(m_centerX, m_centerY), m_innerBorderRadius * 3 / 5, m_innerBorderRadius * 3 / 5);

    //! Стрелка
    p.save();
    int angle = valueToDegrees(m_value);
    QPoint arrowTop = getPointOnCircleByAngle(angle);
    pen.setColor(m_arrowColor);
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(5);
    p.setPen(pen);
    p.drawLine(m_centerX, m_centerY, arrowTop.x(), arrowTop.y());
    pen.setStyle(Qt::NoPen);
    p.restore();

    //! Внутренняя часть стрелочного механизма
    QColor darkCenter(m_panelColor.darker(120));
    QRadialGradient glassGradientCenter(QPoint(m_centerX, m_centerY), m_half / 10 );
    glassGradientCenter.setColorAt(0, m_panelColor);
    glassGradientCenter.setColorAt(1, darkCenter);
    QBrush glassBrushCenter(glassGradientCenter);
    p.setBrush(glassBrushCenter);
    p.drawEllipse(QPoint(m_centerX, m_centerY), m_half / 10, m_half / 10);

    //! Значение числом
    drawValue(p);
}


void CircleNormIndicator::drawNorm(const CircleNormIndicator::NormKind nk, QPainter &p)
{
    int width = m_innerBorderRadius / 5;

    int radius;
    int condNormHi, condNormLo;
    int normHi, normLo;
    if (nk == nkGroup)
    {
        radius = m_innerBorderRadius;
        condNormHi = m_groupNorms.condNormHi;
        condNormLo = m_groupNorms.condNormLo;
        normHi = m_groupNorms.normValHi;
        normLo = m_groupNorms.normValLo;
    }
    else
    {
        radius = m_innerBorderRadius - width;
        condNormHi = m_personalNorms.condNormHi;
        condNormLo = m_personalNorms.condNormLo;
        normHi = m_personalNorms.normValHi;
        normLo = m_personalNorms.normValLo;
    }

    QRect r(m_centerX - radius, m_centerY - radius, (radius) * 2, (radius) * 2);

    //! 0 - не норма, 1 - условная норма, 2 - норма
    //! Если заданы верхние границы, равные maxVal, то норма односторонняя
    for (int i = 0; i < 3; ++i)
    {
        QRadialGradient normGradient(QPoint(m_centerX, m_centerY), radius);
        normGradient.setColorAt(0, m_colorNormLight.value(i));
        normGradient.setColorAt(1, m_colorNormDark.value(i));
        QBrush normBrush(normGradient);
        p.setBrush(normBrush);
        if (i == 0)
            p.drawPie(r, 300 * 16, 300 * 16);
        else
        if (i == 1)
        {
            int u = valueToDegrees(condNormHi);
            int d = valueToDegrees(condNormLo);
            d = getDistanceDegrees(u, d);
            p.drawPie(r, u * 16, d * 16);
        }
        else
        if (i == 2)
        {
            int u = valueToDegrees(normHi);
            int d = valueToDegrees(normLo);
            d = getDistanceDegrees(u, d);
            p.drawPie(r, u * 16, d * 16);
        }
    }

}

int CircleNormIndicator::valueToDegrees(const int v) const
{
    int retval = v * 300 / (m_maxVal - m_minVal);
    retval = 240 - retval;
    if (retval < 0)
        retval = 360 + retval;
    return retval;
}

int CircleNormIndicator::getDistanceDegrees(const int u, const int d) const
{
    if (u >= 300)
        return (360 - u) + d;
    else
        return d - u;
}

QPoint CircleNormIndicator::getPointOnCircleByAngle(const int angle) const
{
    QPoint retval;
    int size = m_half - m_innerBorderWidth * 2;
    float ar = angle * 3.14 / 180;
    if (angle >= 0 && angle < 90)
    {
        retval.setX(m_centerX + fabs(size * cos(ar)));
        retval.setY(m_centerY - fabs(size * sin(ar)));
    }
    else
    if (angle >= 90 && angle < 180)
    {
        retval.setX(m_centerX - fabs(size * sin(ar - 3.14/2)));
        retval.setY(m_centerY - fabs(size * cos(ar - 3.14/2)));
    }
    else
    if (angle >= 180 && angle <= 240)
    {
        retval.setX(m_centerX - fabs(size * cos(ar - 3.14)));
        retval.setY(m_centerY + fabs(size * sin(ar - 3.14)));
    }
    else
    if (angle >= 300 && angle <= 360)
    {
        retval.setX(m_centerX + fabs(size * cos(3.14 * 2 - ar)));
        retval.setY(m_centerY + fabs(size * sin(3.14 * 2 - ar)));
    }
    else
    {
        retval.setX(m_centerX);
        retval.setY(m_centerY);
    }
    return retval;
}

void CircleNormIndicator::drawValue(QPainter &p)
{
    //! Внешняя рамка
    int size = m_half / 3;
    QRadialGradient outerRingGradient(QPoint(m_centerX - size / 2, m_centerY + m_half / 2), size / 2);
    outerRingGradient.setColorAt(0, m_lightColor);
    outerRingGradient.setColorAt(1, m_shadowColor);
    QBrush outerRingBrush(outerRingGradient);
    p.setBrush(outerRingBrush);
    p.drawRect(m_centerX - size / 2 - size / 10, m_centerY + m_half / 2 - size / 10, size + size / 5, size + size / 5);

    //! Внутренняя рамка (с наклоном внутрь)
    QRadialGradient innerRingGradient(QPoint(m_centerX - size / 2, m_centerY + m_half / 2), size / 2);
    innerRingGradient.setColorAt(0, m_shadowColor);
    innerRingGradient.setColorAt(1, m_lightColor);
    QBrush innerRingBrush(innerRingGradient);
    p.setBrush(innerRingBrush);
    p.drawRect(m_centerX - size / 2 - size / 20, m_centerY + m_half / 2 - size / 20, size + size / 10, size + size / 10);

    p.setBrush(QBrush(Qt::black));
    p.drawRect(m_centerX - size / 2, m_centerY + m_half / 2, size, size);

    //! Верхний блик
    QLinearGradient topTeflexGradient(QPoint(m_centerX - size / 2, m_centerY + m_half / 2 + size / 2),
                                      QPoint(m_centerX - size / 2, m_centerY + m_half / 2));
    topTeflexGradient.setColorAt(0, m_topReflexUpColor);
    topTeflexGradient.setColorAt(1, m_topReflexDownColor);
    QBrush topReflexbrush(topTeflexGradient);
    p.setBrush(topReflexbrush);
    p.drawEllipse(QPoint(m_centerX - size / 2, m_topReflexY), m_topReflexWidth, m_topReflexHeight);

    //! Значение
    p.save();
    p.setPen(QPen(Qt::yellow));
    QFont font = p.font();
    font.setPixelSize(size / 2);
    font.setBold(true);
    p.setFont(font);
    p.drawText(m_centerX - size / 2, m_centerY + m_half / 2, size, size,
               Qt::AlignCenter, QString::number(m_value));
    p.restore();
}
