#include "aanalyserapplication.h"

#include "mainwindow.h"

AAnalyserApplication::AAnalyserApplication(int &argc, char **argv, int flags)
    : QApplication(argc, argv, flags)
{

}

void AAnalyserApplication::setMainWindow(QMainWindow *mw)
{
    m_mw = mw;
}

void AAnalyserApplication::showClientPage(const QString &uidPage)
{
    static_cast<MainWindow*>(m_mw)->showClientPage(uidPage);
}
