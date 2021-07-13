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

    EvolventaCalculator* m_calculator {nullptr};
};

#endif // EVOLVENTAVISUALIZE_H
