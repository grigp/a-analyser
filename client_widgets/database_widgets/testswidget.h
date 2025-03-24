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
    explicit TestsWidget(QWidget *parent = nullptr);
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

    void on_selectAllTests();
    void on_unselectAllTests();

    void selectResult();
    void selectDynamic();

    void signalExport();

private:
    Ui::TestsWidget *ui;

    void setEnabledButtons(const bool isEnabled);

    void restoreVisibleWidget();
    void saveVisibleWidget(const QString &value);

    QString m_selectedTestUid {""};
};

#endif // TESTSWIDGET_H
