#include "kerdoapplication.h"

#include <QDebug>

#include "database.h"
#include "mainwindow.h"

#include <QTimer>

KerdoApplication::KerdoApplication(int &argc, char **argv)
    : QApplication (argc, argv)
{
    setApplicationName("Kerdo");
    setOrganizationName("A-Med");

    //! Это необходимо сделать сдесь, чтоб получить и запомнить uid приложения,
    //! чтобы использовать его дальше, в настройках и файлах
    DataDefines::appCopyUid();

    QTimer::singleShot(100, [=]()
    {
        m_database = new DataBase(this);
        connect(m_database, &DataBase::connected, this, &KerdoApplication::dbConnected);
        connect(m_database, &DataBase::disconnected, this, &KerdoApplication::dbDisconnected);
        connect(m_database, &DataBase::newTest, this, &KerdoApplication::newTest);

        connect(static_cast<MainWindow*>(m_mw), &MainWindow::dataBaseChange, m_database, &DataBase::changeDatabase);
        connect(static_cast<MainWindow*>(m_mw), &MainWindow::dataBaseClear, m_database, &DataBase::clear);
    });

}

KerdoApplication::~KerdoApplication()
{
    if (m_database)
        delete m_database;

}

QMainWindow *KerdoApplication::mainWindow() const
{
    return m_mw;
}

void KerdoApplication::setMainWindow(QMainWindow *mw)
{
    m_mw = mw;
    connect(this, &KerdoApplication::dbConnected, static_cast<MainWindow*>(m_mw), &MainWindow::onDbConnected);
    connect(this, &KerdoApplication::dbDisconnected, static_cast<MainWindow*>(m_mw), &MainWindow::obDbDisconnected);
}

DataBase *KerdoApplication::getDB()
{
    return m_database;
}

void KerdoApplication::changeDatabase(const QString &dataBaseFolder)
{
    m_database->changeDatabase(dataBaseFolder);
}

DataDefines::PatientKard KerdoApplication::getSelectedPatient() const
{
    DataDefines::PatientKard kard;
    if (m_patientUid != "" && m_database)
        m_database->getPatient(m_patientUid, kard);
    return kard;
}

void KerdoApplication::doSelectPatient(const QString &uid)
{
    m_patientUid = uid;
    emit selectPatient(m_patientUid);
}
