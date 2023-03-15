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

public:
    explicit SKGWidget(QWidget *parent = nullptr);
    ~SKGWidget() override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::SKGWidget *ui;
};

#endif // SKGWIDGET_H
