#ifndef TESTSWIDGET_H
#define TESTSWIDGET_H

#include <QWidget>

namespace Ui {
class TestsWidget;
}

class TestsModel;
class TestProxyModel;

class TestsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TestsWidget(QWidget *parent = 0);
    ~TestsWidget();

    void onDbConnect();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    /*!
     * \brief Проведение тестирования
     */
    void runTest();

    /*!
     * \brief Выбор теста для просмотра результатов
     * \param index - uid теста
     */
    void selectTest(const QModelIndex &index);

private:
    Ui::TestsWidget *ui;

    TestsModel *m_mdlTest;
    TestProxyModel *m_pmdlTest;
};

#endif // TESTSWIDGET_H
