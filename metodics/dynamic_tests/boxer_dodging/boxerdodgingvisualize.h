#ifndef BOXERDODGINGVISUALIZE_H
#define BOXERDODGINGVISUALIZE_H

#include <QWidget>
#include <QStandardItem>

#include "boxerdodgingdefines.h"

class DynamicDiagram;

namespace Ui {
class BoxerDodgingVisualize;
}

class BoxerDodgingCalculator;

/*!
 * \brief Класс фрейма визуализации результатов тренажера с уклонением для боксеров BoxerDodgingVisualize class
 */
class BoxerDodgingVisualize : public QWidget
{
    Q_OBJECT

public:
    explicit BoxerDodgingVisualize(QWidget *parent = nullptr);
    ~BoxerDodgingVisualize() override;

    void setTest(const QString &testUid);

private:
    Ui::BoxerDodgingVisualize *ui;

    /*!
     * \brief Показывает диаграмму в зоне
     * \param diag - диаграмма
     * \param title - заголовок
     * \param leftFctUid, rightFctUid, aheadFctUid, backFctUid - uid показателей
     */
    void showDiagram(DynamicDiagram* diag,
                     const QString title,
                     const QString leftFctUid,
                     const QString rightFctUid,
                     const QString aheadFctUid,
                     const QString backFctUid);

    /*!
     * \brief Показывает таблицу показателей
     */
    void showTable();

    QList<QStandardItem*> getLineItems(const QString title,
                                       const QString latFctUid,
                                       const QString timeFctUid,
                                       const QString amplFctUid,
                                       const QString errFctUid,
                                       const BoxerDodgingDefines::Stages stage);

    BoxerDodgingCalculator* m_calculator {nullptr};
};

#endif // BOXERDODGINGVISUALIZE_H
