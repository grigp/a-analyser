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

    void onDbConnect();


private slots:
    /*!
     * \brief Проведение тестирования
     */
    void runTest();

    /*!
     * \brief Команда на удаление теста
     */
    void deleteTest();

    void selectResult();
    void selectDynamic();

private:
    Ui::TestsWidget *ui;

    void restoreVisibleWidget();
    void saveVisibleWidget(const QString &value);
};

#endif // TESTSWIDGET_H
