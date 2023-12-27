#ifndef MANCOORDPPVISUALWIDGET_H
#define MANCOORDPPVISUALWIDGET_H

#include <QWidget>
#include <QJsonObject>
#include <QUuid>

#include "ppvisual.h"
#include "generalcoorddefines.h"

namespace Ui {
class ManCoordPPVisualWidget;
}

class GeneralCoordFactors;
class CoordFactorWidget;

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

    /*!
     * \brief Группа функций добавляет виджеты показателей на главный.
     * Введены для рефакторинга
     */
    void addWgtCapacitySetPosAfterShiftUid();
    void addWgtCapacityRitmMovingUid();
    void addWgtMotionAccuracyBeginUid();
    void addWgtFctFinalAccuracyUid();
    void addWgtProcessPerformAccuracyUid();
    void addWgtAmplitudePerformAccuracyUid();
    void addWgtOrientInSpaceUid();
    void addWgtCapacityRepeatMovingUid();

    void addComponent(CoordFactorWidget* wgt, const GeneralCoordDefines::DiapValue value, const QString& name);


    QString m_stepDevTestUid {QUuid().toString()};    ///< uid теста "Прирост" (ступ. отклонение)
    QString m_stepOffsTestUid {QUuid().toString()};   ///< uid теста "Ступени" (ступ. воздействие)
    QString m_evolventaTestUid {QUuid().toString()};  ///< uid теста "Эвольвента"
    QString m_triangleTestUid {QUuid().toString()};   ///< uid теста "Треугольник"

    GeneralCoordFactors *m_factors {nullptr};
};

#endif // MANCOORDPPVISUALWIDGET_H
