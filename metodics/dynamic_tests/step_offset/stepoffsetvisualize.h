#ifndef STEPOFFSETVISUALIZE_H
#define STEPOFFSETVISUALIZE_H

#include <QWidget>
#include <QPrinter>

namespace Ui {
class StepOffsetVisualize;
}

class StepOffsetCalculator;

/*!
 * \brief Класс виджета показа результатов теста "Ступени" StepOffsetVisualize class
 */
class StepOffsetVisualize : public QWidget
{
    Q_OBJECT

public:
    explicit StepOffsetVisualize(QWidget *parent = nullptr);
    ~StepOffsetVisualize();

    void setTest(const QString &testUid);

    /*!
     * \brief Печать отчета о результатах теста
     * \param printer - принтер
     * \param testUid - uid теста
     */
    static void print(QPrinter *printer, const QString &testUid);

private slots:
    void curPageChanged(int pageIdx);
    void splitterMoved(int pos,int index);

private:
    Ui::StepOffsetVisualize *ui;

    void saveSplitterPosition();
    void restoreSplitterPosition();

    /*!
     * \brief Показывает переходные процессы
     */
    void showTransients();

    /*!
     * \brief Показывает таблицу показателей
     */
    void showTable();

    /*!
     * \brief Показывает параметры теста
     */
    void showParams();

    /*!
     * \brief Показывает заключение
     */
    void showConslution();

    /*!
     * \brief Показывает заключение по стратегии
     */
    void showConslutionStrategy();

    StepOffsetCalculator* m_calculator {nullptr};
};

#endif // STEPOFFSETVISUALIZE_H
