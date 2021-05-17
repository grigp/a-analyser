#ifndef CROSSVISUALIZE_H
#define CROSSVISUALIZE_H

#include <QWidget>

namespace Ui {
class CrossVisualize;
}

class CrossCalculator;

/*!
 * \brief Класс виджета показа результатов теста на устойчивость CrossVisualize class
 */
class CrossVisualize : public QWidget
{
    Q_OBJECT

public:
    explicit CrossVisualize(QWidget *parent = nullptr);
    ~CrossVisualize();

    void setTest(const QString &testUid);

private:
    Ui::CrossVisualize *ui;

    CrossCalculator* m_calculator {nullptr};
};

#endif // CROSSVISUALIZE_H
