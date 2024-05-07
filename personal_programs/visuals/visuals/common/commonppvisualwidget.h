#ifndef COMMONPPVISUALWIDGET_H
#define COMMONPPVISUALWIDGET_H

#include <QWidget>

#include "ppvisual.h"

namespace Ui {
class CommonPPVisualWidget;
}

/*!
 * \brief Класс виджета визуализатора ИП - анализ результатов ИП The CommonPPVisualWidget class
 */
class CommonPPVisualWidget : public PPVisual
{
    Q_OBJECT

public:
    explicit CommonPPVisualWidget(PPVisualDescriptor* visual, QJsonObject &objPP, QWidget *parent = nullptr);
    ~CommonPPVisualWidget() override;

    bool isValid() override;
    void calculate() override;
    void print() override;

private:
    Ui::CommonPPVisualWidget *ui;
};

#endif // COMMONPPVISUALWIDGET_H
