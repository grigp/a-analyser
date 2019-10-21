#ifndef RGBLED_H
#define RGBLED_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QGradient>

namespace Ui {
class RGBLed;
}

class RGBLed : public QWidget
{
    Q_OBJECT

public:
    explicit RGBLed(QWidget *parent = 0);
    ~RGBLed();

    void setColor(const QColor &color);

protected:
    void resizeEvent(QResizeEvent *event);

    void paintEvent(QPaintEvent *event);

    QColor ledColor;
    QColor lightColor;
    QColor shadowColor;
    QColor ringShadowDarkColor;
    QColor ringShadowMedColor;
    QColor ringShadowLightColor;
    QColor topReflexUpColor;
    QColor topReflexDownColor;
    QColor bottomReflexCenterColor;
    QColor bottomReflexSideColor;

private:
    Ui::RGBLed *ui;

    void drawLed(const QColor &color);

    int height;
    int width;
    int minDim;
    int half;
    int centerX;
    int centerY;
    QRect drawingRect;

    int outerBorderWidth;
    int innerBorderWidth;
    int outerBorderRadius;
    int innerBorderRadius;
    int topReflexY;
    int bottomReflexY;
    int topReflexWidth;
    int topReflexHeight;
    int bottomReflexWidth;
    int bottomReflexHeight;
};

#endif // RGBLED_H
