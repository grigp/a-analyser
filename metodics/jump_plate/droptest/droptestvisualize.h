#ifndef DROPTESTVISUALIZE_H
#define DROPTESTVISUALIZE_H

#include <QWidget>

namespace Ui {
class DropTestVisualize;
}

class DropTestCalculator;

/*!
 * \brief Виджет показа результатов дроп теста DropTestVisualize class
 */
class DropTestVisualize : public QWidget
{
    Q_OBJECT

public:
    explicit DropTestVisualize(QWidget *parent = 0);
    ~DropTestVisualize();

    void setTest(const QString &testUid);

private:
    Ui::DropTestVisualize *ui;

    int getPatientMassa(const QString &testUid);

    DropTestCalculator* m_calculator {nullptr};
};

#endif // DROPTESTVISUALIZE_H
