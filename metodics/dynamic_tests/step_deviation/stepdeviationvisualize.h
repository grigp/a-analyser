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

/*!
 * \brief Класс виджета показа результатов теста со ступенчатым отклонением The StepDeviationVisualize class
 */
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

    /*!
     * \brief Прорисовывает предварительные результаты с помощью заданного рисователя
     * \param painter - рисователь
     * \param rect - зона прорисовки
     * \param testUid - uid теста
     */
    static void paintPreview(QPainter* painter, QRect& rect, const QString& testUid, StepDeviationCalculator* calculator);

private:
    Ui::StepDeviationVisualize *ui;

    static void printGraph(QPainter *painter, const QRect& rect, StepDeviationVisualize* visual, double ratio);

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
