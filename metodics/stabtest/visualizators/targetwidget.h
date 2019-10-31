#ifndef TARGETWIDGET_H
#define TARGETWIDGET_H

#include <QWidget>

namespace Ui {
class TargetWidget;
}

class TargetCalculator;

/*!
 * \brief Класс виджета визуализатора мишени в рамках стабилографического теста TargetWidget class
 */
class TargetWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TargetWidget(QWidget *parent = 0);
    ~TargetWidget();

    /*!
     * \brief Метод расчета и отображения данных
     * \param testUid - uid теста
     */
    void calculate(TargetCalculator *calculator, const QString &testUid);

private:
    Ui::TargetWidget *ui;

    void showSKG(TargetCalculator *calculator, const QString &testUid) const;
    void showDiagram(TargetCalculator *calculator, const QString &testUid) const;
};

#endif // TARGETWIDGET_H
