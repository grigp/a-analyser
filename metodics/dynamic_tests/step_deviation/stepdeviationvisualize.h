#ifndef STEPDEVIATIONVISUALIZE_H
#define STEPDEVIATIONVISUALIZE_H

#include <QWidget>
#include <QPrinter>
#include <QStandardItemModel>

namespace Ui {
class StepDeviationVisualize;
}

class StepDeviationCalculator;
class AreaGraph;
class DynamicDiagram;

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

    static void printGraph(QPainter *painter, const QRect& rect, StepDeviationVisualize* visual, double ratio);

    static void printDiag(QPainter *painter, const QRect& rect,
//                          StepDeviationVisualize* visual,
                          const double ratio,
                          QVector<double> &data, const QString& title);

    /*!
     * \brief Построение графика прироста шагов
     */
    void showGraph();

    /*!
     * \brief Вывод таблицы показателей
     */
    void showTable();

    /*!
     * \brief Вывод диаграмм
     */
    void showDiags();

    /*!
     * \brief Вывод напоравления
     */
    void showDirection();

    StepDeviationCalculator* m_calculator {nullptr};

    AreaGraph *m_wgtGraph {nullptr};
    DynamicDiagram *m_wgtGrowth {nullptr};
    DynamicDiagram *m_wgtLength {nullptr};
    QStandardItemModel *m_mdlFactors {nullptr};
    QString m_directionText {""};
};

#endif // STEPDEVIATIONVISUALIZE_H
