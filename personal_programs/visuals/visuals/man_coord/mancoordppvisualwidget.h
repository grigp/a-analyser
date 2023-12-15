#ifndef MANCOORDPPVISUALWIDGET_H
#define MANCOORDPPVISUALWIDGET_H

#include <QWidget>
#include <QJsonObject>

#include "ppvisual.h"

namespace Ui {
class ManCoordPPVisualWidget;
}

/*!
 * \brief Класс виджета визуализатора ИП - оценка координационных способностей человека The ManCoordPPVisualWidget class
 */
class ManCoordPPVisualWidget : public PPVisual
{
    Q_OBJECT

public:
    explicit ManCoordPPVisualWidget(PPVisualDescriptor* visual, QJsonObject &objPP, QWidget *parent = nullptr);
    ~ManCoordPPVisualWidget() override;

    bool isValid() override;
    void calculate() override;

private:
    Ui::ManCoordPPVisualWidget *ui;

    QJsonObject m_objPP {QJsonObject()};
};

#endif // MANCOORDPPVISUALWIDGET_H
