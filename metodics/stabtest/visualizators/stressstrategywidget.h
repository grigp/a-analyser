#ifndef STRESSSTRATEGYWIDGET_H
#define STRESSSTRATEGYWIDGET_H

#include <QWidget>
#include <QPrinter>

namespace Ui {
class StressStrategyWidget;
}

class StressStrategyCalculator;

/*!
 * \brief Класс виджета визуализатора оценки стрессовой стратегии в рамках стабилографического теста StressStrategyWidget class
 */
class StressStrategyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StressStrategyWidget(QWidget *parent = nullptr);
    ~StressStrategyWidget() override;

    /*!
     * \brief Метод расчета и отображения данных
     * \param testUid - uid теста
     */
    void calculate(StressStrategyCalculator *calculator, const QString &testUid);

    /*!
     * \brief Печать отчета о результатах теста
     * \param printer - принтер
     * \param testUid - uid теста
     */
    void print(QPrinter *printer, const QString &testUid);

private:
    Ui::StressStrategyWidget *ui;
};

#endif // STRESSSTRATEGYWIDGET_H
