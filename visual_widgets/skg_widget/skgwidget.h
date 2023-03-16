#ifndef SKGWIDGET_H
#define SKGWIDGET_H

#include <QWidget>

#include "skgpainter.h"

namespace Ui {
class SKGWidget;
}

/*!
 * \brief Класс виджета статокинезиграммы The SKGWidget class
 * Реализован с помощью SKGPainter
 */
class SKGWidget : public QWidget, public SKGPainter
{
    Q_OBJECT

    Q_PROPERTY(QColor skg_color READ colorSKG WRITE setColorSKG DESIGNABLE true)
    Q_PROPERTY(QColor ellipse_color READ colorEllipse WRITE setColorEllipse DESIGNABLE true)
    Q_PROPERTY(QColor marker_color READ colorMarker WRITE setColorMarker DESIGNABLE true)
    Q_PROPERTY(QColor platforms_color READ colorPlatforms WRITE setColorPlatforms DESIGNABLE true)

public:
    explicit SKGWidget(QWidget *parent = nullptr);
    ~SKGWidget() override;

protected:
    void paintEvent(QPaintEvent *event) override;

    void doUpdate() override;

private:
    Ui::SKGWidget *ui;
};

#endif // SKGWIDGET_H
