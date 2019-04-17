#ifndef TESTSWIDGET_H
#define TESTSWIDGET_H

#include <QWidget>

namespace Ui {
class TestsWidget;
}

class TestsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TestsWidget(QWidget *parent = 0);
    ~TestsWidget();

private slots:
    /*!
     * \brief Проведение тестирования
     */
    void runTest();

private:
    Ui::TestsWidget *ui;
};

#endif // TESTSWIDGET_H
