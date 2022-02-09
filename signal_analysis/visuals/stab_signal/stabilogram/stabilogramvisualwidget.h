#ifndef STABILOGRAMVISUALWIDGET_H
#define STABILOGRAMVISUALWIDGET_H

#include <QWidget>

#include "visual.h"

namespace Ui {
class StabilogramVisualWidget;
}

/*!
 * \brief Класс виджета визуализатора стабилограммы The StabilogramVisualWidget class
 */
class StabilogramVisualWidget : public Visual
{
    Q_OBJECT

public:
    explicit StabilogramVisualWidget(BaseVisual* visual, QWidget *parent = nullptr);
    ~StabilogramVisualWidget();

    void calculate() override;


private:
    Ui::StabilogramVisualWidget *ui;
};

#endif // STABILOGRAMVISUALWIDGET_H
