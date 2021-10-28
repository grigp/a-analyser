#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>

class DataBaseResultWidget;
class ExecuteWidget;
class SettingsValue;

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
    void dataBaseClear();
    void deleteTests();
    void dataBaseCreate();

public slots:
    void onDbConnected();

    void obDbDisconnected();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onDeviceControl();
    void onSettings();

    void onDataBaseProperty();
    void onDataBaseExport();
    void onDataBaseImport();
    void onDataBaseSelect();
    void onDataBaseClear();
    void onDataBaseDelTests();
    void onDataBaseCreate();

    void on_selectDatabase();
    void on_selectColorSheme();

private:
    Ui::MainWindow *ui;

    void initUi(const QString& colorSheme);

    void initMenu();
    QMenu* initDatabaseClearMenu();
    void initSelectDatabaseMenu();

    /*!
     * \brief Создает клиентские виджеты и размещает их на главном окне
     */
    void createClientWidgets();

    QList<QWidget*> m_clientWidgets;
    QString m_currentClientPage = ""; ///< Текущая страница с данными
    QMenu m_menuSelectDatabase;       ///< Меню выбора БД

    QActionGroup* m_agColorShemes {nullptr};
    SettingsValue* m_curColorSheme  {nullptr};  ///< Выбранная цветовая схема
};

#endif // MAINWINDOW_H
