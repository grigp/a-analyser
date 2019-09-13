#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>

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

signals:
    void dataBaseChange(const QString &dataBaseFolder);

public slots:
    void onDbConnected();

    void obDbDisconnected();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onDeviceControl();

    void onDataBaseProperty();
    void onDataBaseExport();
    void onDataBaseImport();
    void onDataBaseSelect();

    void on_selectDatabase();

private:
    Ui::MainWindow *ui;

    void initUi();

    void initMenu();
    void initSelectDatabaseMenu();

    /*!
     * \brief Создает клиентские виджеты и размещает их на главном окне
     */
    void createClientWidgets();

    QList<QWidget*> m_clientWidgets;
    QString m_currentClientPage = ""; ///< Текущая страница с данными
    QMenu m_menuSelectDatabase;       ///< Меню выбора БД

};

#endif // MAINWINDOW_H
