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

private slots:
    void onDeviceControl();

    void onDataBaseProperty();
    void onDataBaseExport();
    void onDataBaseImport();
    void onDataBaseSelect();

private:
    Ui::MainWindow *ui;

    void initUi();

    void initMenu();

    /*!
     * \brief Создает клиентские виджеты и размещает их на главном окне
     */
    void createClientWidgets();

    QList<QWidget*> m_clientWidgets;
    QString m_currentClientPage = ""; ///< Текущая страница с данными

};

#endif // MAINWINDOW_H
