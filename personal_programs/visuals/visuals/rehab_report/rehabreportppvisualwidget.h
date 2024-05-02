#ifndef REHABREPORTPPVISUALWIDGET_H
#define REHABREPORTPPVISUALWIDGET_H

#include <QWidget>

#include "ppvisual.h"

namespace Ui {
class RehabReportPPVisualWidget;
}

/*!
 * \brief Класс виджета визуализатора ИП - отчет по курсу реабилитации The RehabReportPPVisualWidget class
 */
class RehabReportPPVisualWidget : public PPVisual
{
    Q_OBJECT

public:
    explicit RehabReportPPVisualWidget(PPVisualDescriptor* visual, QJsonObject &objPP, QWidget *parent = nullptr);
    ~RehabReportPPVisualWidget() override;

    bool isValid() override;
    void calculate() override;

private:
    Ui::RehabReportPPVisualWidget *ui;

    QString m_uidRomb1 {""};    ///< uid теста Ромберга перед началом процедур
    QString m_uidRomb2 {""};    ///< uid теста Ромберга перед окончанием процедур
    QString m_uidCross1 {""};   ///< uid теста на устойчивость перед началом процедур
    QString m_uidCross2 {""};   ///< uid теста на устойчивость перед окончанием процедур
    QVector<double> m_success;  ///< Динамика результативности по ДП

    /*!
     * \brief Расчет вперед по ДП
     */
    void calculateStraight();
    /*!
     * \brief Расчет назад по ДП
     */
    void calculateReverse();
};

#endif // REHABREPORTPPVISUALWIDGET_H
