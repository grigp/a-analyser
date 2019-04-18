#include "aanalyserapplication.h"

#include <QStandardPaths>
#include <QTimer>
#include <QDebug>

#include "mainwindow.h"
#include "database.h"

AAnalyserApplication::AAnalyserApplication(int &argc, char **argv)
    : QApplication(argc, argv)
{
    setApplicationName("a-analyser");
    setApplicationDisplayName("Физиологические исследования a-analyser");
    setOrganizationName("A-Med");

    QTimer::singleShot(100, [=]()
    {
        m_database = new DataBase(this);
        connect(m_database, &DataBase::connected, this, &AAnalyserApplication::dbConnected);
        connect(m_database, &DataBase::disconnected, this, &AAnalyserApplication::dbDisconnected);
    });
}

AAnalyserApplication::~AAnalyserApplication()
{
    if (m_database)
        delete m_database;
}

void AAnalyserApplication::setMainWindow(QMainWindow *mw)
{
    m_mw = mw;
    connect(this, &AAnalyserApplication::dbConnected, static_cast<MainWindow*>(m_mw), &MainWindow::onDbConnected);

    QString path = QStandardPaths::locate(QStandardPaths::GenericDataLocation, QString(), QStandardPaths::LocateDirectory);
    qDebug() << path + applicationName() + '/';
}

void AAnalyserApplication::showClientPage(const QString &uidPage)
{
    static_cast<MainWindow*>(m_mw)->showClientPage(uidPage);
}

DataBase *AAnalyserApplication::getDB()
{
    return m_database;
}
