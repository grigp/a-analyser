#include "aanalyserapplication.h"

#include <QStandardPaths>
#include <QTimer>
#include <QMessageBox>
#include <QLayout>
#include <QDebug>

#include "mainwindow.h"
#include "metodicsfactory.h"
#include "executewidget.h"
#include "databaseresultwidget.h"
#include "database.h"

AAnalyserApplication::AAnalyserApplication(int &argc, char **argv)
    : QApplication(argc, argv)
    , m_metodics(new MetodicsFactory(this))
{
    setApplicationName("a-analyser");
    setApplicationDisplayName("Физиологические исследования a-analyser");
    setOrganizationName("A-Med");

    QTimer::singleShot(100, [=]()
    {
        m_database = new DataBase(this);
        connect(m_database, &DataBase::connected, this, &AAnalyserApplication::dbConnected);
        connect(m_database, &DataBase::disconnected, this, &AAnalyserApplication::dbDisconnected);
        connect(m_database, &DataBase::newTest, this, &AAnalyserApplication::newTest);
    });
}

AAnalyserApplication::~AAnalyserApplication()
{
    if (m_database)
        delete m_database;
    if (m_metodics)
        delete m_metodics;
}

void AAnalyserApplication::setMainWindow(QMainWindow *mw)
{
    m_mw = mw;
    connect(this, &AAnalyserApplication::dbConnected, static_cast<MainWindow*>(m_mw), &MainWindow::onDbConnected);
}

void AAnalyserApplication::showClientPage(const QString &uidPage)
{
    static_cast<MainWindow*>(m_mw)->showClientPage(uidPage);
}

DataBase *AAnalyserApplication::getDB()
{
    return m_database;
}

DataDefines::PatientKard AAnalyserApplication::getSelectedPatient() const
{
    DataDefines::PatientKard kard;
    if (m_patientUid != "" && m_database)
        m_database->getPatient(m_patientUid, kard);
    return kard;
}

MetodicDefines::MetodicInfo AAnalyserApplication::getSelectedMetodic() const
{
    if (m_metodics && m_metodicUid != "")
        return m_metodics->metodic(m_metodicUid);
    return MetodicDefines::MetodicInfo();
}

MetodicsFactory *AAnalyserApplication::getMetodics()
{
    return m_metodics;
}

void AAnalyserApplication::doSelectPatient(const QString &uid)
{
    m_patientUid = uid;
    emit selectPatient(m_patientUid);
}

void AAnalyserApplication::doSelectMetodic(const QString &uid)
{
    m_metodicUid = uid;
    emit selectMetodic(m_metodicUid);
}

void AAnalyserApplication::executeMetodic()
{
    if (m_patientUid == "")
        QMessageBox::information(m_mw, tr("Предупреждение"), tr("Должен быть выбран пациент"));
    else
    if (m_metodicUid == "")
        QMessageBox::information(m_mw, tr("Предупреждение"), tr("Должна быть выбрана методика"));
    else
    {
        auto widget = static_cast<MainWindow*>(m_mw)->getExecuteWidget();
        if (widget)
        {
            while (QLayoutItem* item = widget->layout()->takeAt(0))
            {
                delete item->widget();
                delete item;
            }
            m_metodics->execute(widget, m_metodicUid);
            showClientPage(ClientWidgets::uidExecuteWidgetUid);
        }
    }
}

void AAnalyserApplication::openTest(const QString &uid)
{

}

void AAnalyserApplication::showDataBase()
{
    showClientPage(ClientWidgets::uidDatabaseResultWidgetUid);
}
