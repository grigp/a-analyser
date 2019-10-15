#include "rgbled.h"
#include "ui_rgbled.h"

RGBLed::RGBLed(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RGBLed),
    ledColor(Qt::green),
    lightColor(QColor(0xE0, 0xE0, 0xE0)),
    shadowColor(QColor(0x70, 0x70, 0x70)),
    ringShadowDarkColor(QColor(0x50, 0x50, 0x50, 0xFF)),
    ringShadowMedColor(QColor(0x50, 0x50, 0x50, 0x20)),
    ringShadowLightColor(QColor(0xEE, 0xEE, 0xEE, 0x00)),
    topReflexUpColor(QColor(0xFF, 0xFF, 0xFF, 0xA0)),
    topReflexDownColor(QColor(0xFF, 0xFF, 0xFF, 0x00)),
    bottomReflexCenterColor(QColor(0xFF, 0xFF, 0xFF, 0x00)),
    bottomReflexSideColor(QColor(0xFF, 0xFF, 0xFF, 0x70))
{
    ui->setupUi(this);
}

RGBLed::~RGBLed()
{
    delete ui;
}

void RGBLed::setColor(const QColor &color)
{
    ledColor = color;
}

void RGBLed::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    height = size().height();
    width = size().width();
    minDim = (height > width) ? width : height;
    half = minDim / 2;
    centerX = width / 2;
    centerY = height / 2;

    outerBorderWidth = minDim / 10;
    innerBorderWidth = minDim / 14;
    outerBorderRadius = half - outerBorderWidth;
    innerBorderRadius = half - (outerBorderWidth + innerBorderWidth);

    topReflexY = centerY
            - (half - outerBorderWidth - innerBorderWidth) / 2;
    bottomReflexY = centerY
            + (half - outerBorderWidth - innerBorderWidth) / 2;
    topReflexHeight = half / 5;
    topReflexWidth = half / 3;
    bottomReflexHeight = half / 5;
    bottomReflexWidth = half / 3;

    drawingRect.setTop((height - minDim) / 2);
    drawingRect.setLeft((width - minDim) / 2);
    drawingRect.setHeight(minDim);
    drawingRect.setWidth(minDim);
}

void RGBLed::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    drawLed(ledColor);
}

void RGBLed::drawLed(const QColor &color)
{
    QPainter p(this);

    QPen pen;
    pen.setStyle(Qt::NoPen);
    p.setPen(pen);

    // Внешнее кольцо (с наклоном наружу, часть выпуклого ободка)
    QRadialGradient outerRingGradient(QPoint(centerX,
                          centerY - outerBorderRadius - (outerBorderWidth / 2)),
                          minDim - (outerBorderWidth / 2));
    outerRingGradient.setColorAt(0, lightColor);
    outerRingGradient.setColorAt(1, shadowColor);
    QBrush outerRingBrush(outerRingGradient);
    p.setBrush(outerRingBrush);
    p.drawEllipse(this->drawingRect);

    // Внутреннее кольцо (с наклоном внутрь)
    QRadialGradient innerRingGradient(QPoint(centerX,
                             centerY + innerBorderRadius + (innerBorderWidth / 2)),
                             minDim - (innerBorderWidth / 2));
    innerRingGradient.setColorAt(0, lightColor);
    innerRingGradient.setColorAt(1, shadowColor);
    QBrush innerRingBrush(innerRingGradient);
    p.setBrush(innerRingBrush);
    p.drawEllipse(QPoint(centerX, centerY),
                  outerBorderRadius, outerBorderRadius);

    // Корпус лампочки-светодиода, «стекло»
    QColor dark(color.darker(120));
    QRadialGradient glassGradient(QPoint(centerX, centerY),
                              innerBorderRadius);
    glassGradient.setColorAt(0, color);
    glassGradient.setColorAt(1, dark);
    QBrush glassBrush(glassGradient);
    p.setBrush(glassBrush);
    p.drawEllipse(QPoint(centerX, centerY),
                  innerBorderRadius,
                  innerBorderRadius);

    // Тень по краю стекла
    QRadialGradient shadowGradient(QPoint(centerX, centerY),
                              innerBorderRadius);
    shadowGradient.setColorAt(0, ringShadowLightColor);
    shadowGradient.setColorAt(0.85, ringShadowMedColor);
    shadowGradient.setColorAt(1, ringShadowDarkColor);
    QBrush shadowBrush(shadowGradient);
    p.setBrush(shadowBrush);
    p.drawEllipse(QPoint(centerX, centerY),
                  innerBorderRadius,
                  innerBorderRadius);

    // Верхний блик
    QLinearGradient topTeflexGradient(QPoint(centerX,
                                     (innerBorderWidth + outerBorderWidth)),
                              QPoint(centerX, centerY));
    topTeflexGradient.setColorAt(0, topReflexUpColor);
    topTeflexGradient.setColorAt(1, topReflexDownColor);
    QBrush topReflexbrush(topTeflexGradient);
    p.setBrush(topReflexbrush);
    p.drawEllipse(QPoint(centerX, topReflexY), topReflexWidth, topReflexHeight);

    // Нижний блик
    QRadialGradient bottomReflexGradient(QPoint(centerX,
                                     bottomReflexY + (bottomReflexHeight / 2)),
                              bottomReflexWidth);
    bottomReflexGradient.setColorAt(0, bottomReflexSideColor);
    bottomReflexGradient.setColorAt(1, bottomReflexCenterColor);
    QBrush bottomReflexBrush(bottomReflexGradient);
    p.setBrush(bottomReflexBrush);
    p.drawEllipse(QPoint(centerX, bottomReflexY),
                  bottomReflexWidth,
                  bottomReflexHeight);
}
