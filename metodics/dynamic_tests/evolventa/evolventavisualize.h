#ifndef EVOLVENTAVISUALIZE_H
#define EVOLVENTAVISUALIZE_H

#include <QWidget>

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

    EvolventaCalculator* m_calculator {nullptr};
};

#endif // EVOLVENTAVISUALIZE_H
