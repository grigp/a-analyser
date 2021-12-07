#ifndef STEPDEVIATIONVISUALIZE_H
#define STEPDEVIATIONVISUALIZE_H

#include <QWidget>
#include <QPrinter>

namespace Ui {
class StepDeviationVisualize;
}

class StepDeviationCalculator;

class StepDeviationVisualize : public QWidget
{
    Q_OBJECT

public:
    explicit StepDeviationVisualize(QWidget *parent = nullptr);
    ~StepDeviationVisualize();

    void setTest(const QString &testUid);

    /*!
     * \brief Печать отчета о результатах теста
     * \param printer - принтер
     * \param testUid - uid теста
     */
    static void print(QPrinter *printer, const QString &testUid);


private:
    Ui::StepDeviationVisualize *ui;

    /*!
     * \brief Построение графика прироста шагов
     */
    void showGraph();

    StepDeviationCalculator* m_calculator {nullptr};
};

#endif // STEPDEVIATIONVISUALIZE_H
