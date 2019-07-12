#ifndef STABTESTVISUALIZE_H
#define STABTESTVISUALIZE_H

#include <QWidget>
#include <QTextEdit>

namespace Ui {
class StabTestVisualize;
}

class TestResultData;

class TestCalculator;

/*!
 * \brief Виджет показа результатов стабилометрического тестирования StabTestVisualize class
 */
class StabTestVisualize : public QWidget
{
    Q_OBJECT

public:
    explicit StabTestVisualize(TestCalculator* calculator, QWidget *parent = 0);
    ~StabTestVisualize();

    void setTest(const QString &testUid);

private:
    Ui::StabTestVisualize *ui;

    TestCalculator* m_calculator;

};

#endif // STABTESTVISUALIZE_H
