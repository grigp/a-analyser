#ifndef EVOLVENTAVISUALIZE_H
#define EVOLVENTAVISUALIZE_H

#include <QWidget>
#include <QPrinter>

namespace Ui {
class EvolventaVisualize;
}

class EvolventaCalculator;

/*!
 * \brief Класс виджета показа результатов теста Эвольвента EvolventaVisualize class
 */
class EvolventaVisualize : public QWidget
{
    Q_OBJECT

public:
    explicit EvolventaVisualize(QWidget *parent = nullptr);
    ~EvolventaVisualize() override;

    void setTest(const QString &testUid);

    /*!
     * \brief Печать отчета о результатах теста
     * \param printer - принтер
     * \param testUid - uid теста
     */
    static void print(QPrinter *printer, const QString &testUid);

private slots:
    /*!
     * \brief Сохраняет таблицу показателей коррекций
     */
    void saveFactorsCorrections();

    /*!
     * \brief Сохраняет эвольвенту
     */
    void saveEvolventa();

private:
    Ui::EvolventaVisualize *ui;

    /*!
     * \brief Показывает эвольвенту
     */
    void showEvolventa();

    /*!
     * \brief Показывает таблицу основных показателей
     */
    void showMainFactorsTable();

    /*!
     * \brief Показывает таблицу показателей коррекций
     */
    void showKorrectionFactorsTable();

    void setTableSpecialStyle();

    /*!
     * \brief Показывает показатели без таблиц
     */
    void showWithoutTableFactors();

    EvolventaCalculator* m_calculator {nullptr};
};

#endif // EVOLVENTAVISUALIZE_H
