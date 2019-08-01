#ifndef STATECHAMPIONSWIDGET_H
#define STATECHAMPIONSWIDGET_H

#include <QWidget>

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
    explicit StateChampionsWidget(QWidget *parent = 0);
    ~StateChampionsWidget();

    /*!
     * \brief Метод расчета и отображения данных
     * \param testUid - uid теста
     */
    void calculate(StateChampionsTestCalculator *calculator, const QString &testUid);

private:
    Ui::StateChampionsWidget *ui;
};

#endif // STATECHAMPIONSWIDGET_H
