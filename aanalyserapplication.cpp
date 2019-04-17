#include "aanalyserapplication.h"

#include <QStandardPaths>
#include <QDebug>

#include "mainwindow.h"

AAnalyserApplication::AAnalyserApplication(int &argc, char **argv, int flags)
    : QApplication(argc, argv, flags)
{
    setApplicationName("a-analyser");
    setApplicationDisplayName("Физиологические исследования a-analyser");
    setOrganizationName("A-Med");
}

void AAnalyserApplication::setMainWindow(QMainWindow *mw)
{
    m_mw = mw;
    QString path = QStandardPaths::locate(QStandardPaths::GenericDataLocation, QString(), QStandardPaths::LocateDirectory);
    qDebug() << path + applicationName() + '/';
}

void AAnalyserApplication::showClientPage(const QString &uidPage)
{
    static_cast<MainWindow*>(m_mw)->showClientPage(uidPage);
}
