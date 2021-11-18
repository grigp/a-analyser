#ifndef TARGETWIDGET_H
#define TARGETWIDGET_H

#include <QWidget>
#include <QPrinter>

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
    explicit TargetWidget(QWidget *parent = nullptr);
    ~TargetWidget() override;

    /*!
     * \brief Метод расчета и отображения данных
     * \param testUid - uid теста
     */
    void calculate(TargetCalculator *calculator, const QString &testUid);

    /*!
     * \brief Печать отчета о результатах теста
     * \param printer - принтер
     * \param testUid - uid теста
     */
    static void print(QPrinter *printer, const QString &testUid);

private:
    Ui::TargetWidget *ui;

    void showSKG(TargetCalculator *calculator, const QString &testUid) const;
    void showDiagram(TargetCalculator *calculator, const QString &testUid) const;
};

#endif // TARGETWIDGET_H
