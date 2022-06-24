#ifndef TESTSWIDGET_H
#define TESTSWIDGET_H

#include <QWidget>
#include <QPrinter>


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
    void onDbDisconnect();


private slots:
    /*!
     * \brief Проведение тестирования
     */
    void runTest();

    /*!
     * \brief Команда на удаление теста
     */
    void deleteTest();

    void editTestProperty();
    void printReport();

    void signalsAnalysis();
    void summaries();
    void summaryAddTest();

    void selectResult();
    void selectDynamic();

    void print(QPrinter* printer);

private:
    Ui::TestsWidget *ui;

    void restoreVisibleWidget();
    void saveVisibleWidget(const QString &value);

    QString m_selectedTestUid {""};
};

#endif // TESTSWIDGET_H
