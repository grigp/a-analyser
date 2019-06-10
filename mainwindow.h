#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class DataBaseResultWidget;
class ExecuteWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /*!
     * \brief Показывает страницу с заданным uid
     * \param uidPage - uid страницы
     */
    void showClientPage(const QString &uidPage);

    QWidget *getExecuteWidget();

public slots:
    void onDbConnected();

    void obDBDisconnected();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;

    void initUi();

    /*!
     * \brief Создает клиентские виджеты и размещает их на главном окне
     */
    void createClientWidgets();

    QList<QWidget*> m_clientWidgets;
    QString m_currentClientPage = ""; ///< Текущая страница с данными

};

#endif // MAINWINDOW_H
