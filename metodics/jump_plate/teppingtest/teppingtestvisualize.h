#ifndef TEPPINGTESTVISUALIZE_H
#define TEPPINGTESTVISUALIZE_H

#include <QWidget>

namespace Ui {
class TeppingTestVisualize;
}

class TeppingTestCalculator;

/*!
 * \brief Виджет показа результатов теппинг - теста TeppingTestVisualize class
 */
class TeppingTestVisualize : public QWidget
{
    Q_OBJECT

public:
    explicit TeppingTestVisualize(QWidget *parent = 0);
    ~TeppingTestVisualize();

    void setTest(const QString &testUid);

private:
    Ui::TeppingTestVisualize *ui;

    TeppingTestCalculator* m_calculator {nullptr};
};

#endif // TEPPINGTESTVISUALIZE_H
