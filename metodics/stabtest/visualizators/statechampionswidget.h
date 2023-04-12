#ifndef STATECHAMPIONSWIDGET_H
#define STATECHAMPIONSWIDGET_H

#include <QWidget>
#include <QPrinter>

namespace Ui {
class StateChampionsWidget;
}

class StateChampionsTestCalculator;

/*!
 * \brief Класс виджета визуализатора теста по методике "Стань чемпионом" в рамках стабилографического теста StateChampionsWidget class
 */
class StateChampionsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StateChampionsWidget(QWidget *parent = nullptr);
    ~StateChampionsWidget() override;

    /*!
     * \brief Метод расчета и отображения данных
     * \param testUid - uid теста
     */
    void calculate(StateChampionsTestCalculator *calculator, const QString &testUid);

    /*!
     * \brief Печать отчета о результатах теста
     * \param printer - принтер
     * \param testUid - uid теста
     */
    static void print(QPrinter *printer, const QString &testUid);

private:
    Ui::StateChampionsWidget *ui;

    QString m_sSquareProbe1 {""};
    QString m_sSquareProbe2 {""};
    QString m_sSquareProbe3 {""};
    QString m_sSpeedProbe1 {""};
    QString m_sSpeedProbe2 {""};
    QString m_sSpeedProbe3 {""};
    QString m_sKFRProbe3 {""};
    QString m_sRombKoefValue {""};
};

#endif // STATECHAMPIONSWIDGET_H
