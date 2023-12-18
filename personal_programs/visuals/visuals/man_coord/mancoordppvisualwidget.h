#ifndef MANCOORDPPVISUALWIDGET_H
#define MANCOORDPPVISUALWIDGET_H

#include <QWidget>
#include <QJsonObject>
#include <QUuid>

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

    QString m_stepDevTestUid {QUuid().toString()};    ///< uid теста "Прирост" (ступ. отклонение)
    QString m_stepOffsTestUid {QUuid().toString()};   ///< uid теста "Ступени" (ступ. воздействие)
    QString m_evolventaTestUid {QUuid().toString()};  ///< uid теста "Эвольвента"
    QString m_triangleTestUid {QUuid().toString()};   ///< uid теста "Треугольник"
};

#endif // MANCOORDPPVISUALWIDGET_H
