#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QStack>

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
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    /*!
     * \brief Показывает страницу с заданным uid
     * \param uidPage - uid страницы
     */
    void showClientPage(const QString &uidPage);

    /*!
     * \brief Восстанавливает предыдущую страницу из стека последних показанных страниц
     * \return Возвращает true, если было, что восстанавливать
     */
    bool restoreClientPage();

    /*!
     * \brief Возвращает uid текущей страницы
     * Если нет, то возвращает QUuid().toString(), но такого не должно быть
     */
    QString currentClientPage();

    QWidget *getExecuteWidget();

    /*!
     * \brief Инициализирует полосу процесса, показывает ее
     * \param title - заголовок прогресса
     * \param begin - начальная точка
     * \param end - конечная точка
     * \param stage - название этапа
     */
    void initProgress(const QString &title, const int begin = 0, const int end = 100, const QString &stage = "");

    /*!
     * \brief Устанавливает позицию процесса
     * \param pos - значение позиции
     * \param stage - название этапа
     */
    void setProgressPosition(const int pos, const QString &stage = "");

    /*!
     * \brief Прячет полосу процесса
     */
    void doneProgress();

    /*!
     * \brief Возвращает uid текущую показываемого клиентского виджета
     */
    QString currentClientPage() const {return m_currentClientPage;}

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
    void showEvent(QShowEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onDeviceControl();
    void onDeviceSetup();
    void onSettings();
    void onAbout();

    void onDataBaseProperty();
    void onDataBaseExport();
    void onDataBaseImport();
    void onDataBaseSelect();
    void onDataBaseClear();
    void onDataBaseDelTests();
    void onDataBaseCreate();

    void on_selectDatabase();
    void on_selectColorSheme();

    void on_viewDataBase();
    void on_viewPersonalProgram();

    void onSummariesShow();
    void onAddTestToSymmary();
    void onSummaryBuild();
    void onSummariesBrowse();

    void on_editPersonalPrograms();
    void on_editDailyPrograms();
    void on_PPAssign();
    void on_PPCancel();

    void on_clearGeometry();

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
    QString m_currentClientPage = "";       ///< Текущая страница с данными
    QMenu* m_menuSelectDatabase {nullptr};  ///< Меню выбора БД

    QActionGroup* m_agColorShemes {nullptr};
    QActionGroup* m_agViewsMain {nullptr};
    SettingsValue* m_curColorSheme  {nullptr};  ///< Выбранная цветовая схема

    QStack<QString> m_lastPages;   ///< Последние открытые страницы
};

#endif // MAINWINDOW_H
