#ifndef DOPUSKWIDGET_H
#define DOPUSKWIDGET_H

#include <QWidget>

namespace Ui {
class DopuskWidget;
}

class DopuskCalculator;

/*!
 * \brief Класс виджета визуализатора допускового контроля в рамках стабилографического теста DopuskWidget class
 */
class DopuskWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DopuskWidget(QWidget *parent = 0);
    ~DopuskWidget();

    /*!
     * \brief Метод расчета и отображения данных
     * \param testUid - uid теста
     */
    void calculate(DopuskCalculator *calculator, const QString &testUid);

private:
    Ui::DopuskWidget *ui;
};

#endif // DOPUSKWIDGET_H
