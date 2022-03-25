#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>

namespace Ui {
class MainWindow;
}

class PatientsModel;
class PatientsProxyModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onDbConnected();
    void obDbDisconnected();

private slots:
    void addMan();
    void delMan();
    void editMan();

    void selectPatient(QModelIndex index);

signals:
    void dataBaseChange(const QString &dataBaseFolder);
    void dataBaseClear();
    void deleteTests();
    void dataBaseCreate();

private:
    Ui::MainWindow *ui;

    PatientsModel *m_mdlPatients;
    PatientsProxyModel *m_pmdlPatients;
};

#endif // MAINWINDOW_H
