#ifndef TESTSWIDGET_H
#define TESTSWIDGET_H

#include <QWidget>

namespace Ui {
class TestsWidget;
}

class TestsModel;

class TestsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TestsWidget(QWidget *parent = 0);
    ~TestsWidget();

    void onDbConnect();

private slots:
    /*!
     * \brief Проведение тестирования
     */
    void runTest();

private:
    Ui::TestsWidget *ui;

    TestsModel *m_mdlTest;
};

#endif // TESTSWIDGET_H
