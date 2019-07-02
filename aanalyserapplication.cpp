#include "aanalyserapplication.h"

#include <QStandardPaths>
#include <QTimer>
#include <QMessageBox>
#include <QLayout>
#include <QDebug>

#include "mainwindow.h"
#include "metodicsfactory.h"
#include "driversfactory.h"
#include "executewidget.h"
#include "databaseresultwidget.h"
#include "database.h"

AAnalyserApplication::AAnalyserApplication(int &argc, char **argv)
    : QApplication(argc, argv)
    , m_metodics(new MetodicsFactory(this))
    , m_drivers(new DriversFactory(this))
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
    if (m_drivers)
        delete m_drivers;
}

QMainWindow *AAnalyserApplication::mainWindow() const
{
    return m_mw;
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

void AAnalyserApplication::doSelectTest(const QString &uid)
{
    //! Возможно много лишних срабатываний
    if (uid != m_testUid)
    {
        m_testUid = uid;
        emit selectTest(m_testUid);
    }
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
    Q_UNUSED(uid);

}

void AAnalyserApplication::showDataBase()
{
    showClientPage(ClientWidgets::uidDatabaseResultWidgetUid);
}

QStringList AAnalyserApplication::getDrivers() const
{
    if (m_drivers)
        return m_drivers->getDrivers();
    return QStringList();
}

QString AAnalyserApplication::getDriverName(const QString &drvUid) const
{
    if (m_drivers)
        return m_drivers->getDriverName(drvUid);
    return QString("");
}

QList<Connection> AAnalyserApplication::getConnections() const
{
    if (m_drivers)
        return m_drivers->getConnections();
    return QList<Connection>();
}

void AAnalyserApplication::addConnection(const bool active,
                                         const QString &drvUid,
                                         const QJsonObject &params,
                                         const DeviceProtocols::Ports port,
                                         const QString &comment)
{
    if (m_drivers)
        m_drivers->addConnection(active, drvUid, params, port, comment);
}

void AAnalyserApplication::moveConnectionUp(const int connectIdx)
{
    if (m_drivers)
        m_drivers->moveConnectionUp(connectIdx);
}

void AAnalyserApplication::moveConnectionDown(const int connectIdx)
{
    if (m_drivers)
        m_drivers->moveConnectionDown(connectIdx);
}

void AAnalyserApplication::deleteConnection(const int connectIdx)
{
    if (m_drivers)
        m_drivers->deleteConnection(connectIdx);
}

void AAnalyserApplication::dataChangedConnection(const int connectIdx, const int paramIdx, const QVariant value)
{
    if (m_drivers)
        m_drivers->dataChangedConnection(connectIdx, paramIdx, value);
}

bool AAnalyserApplication::editParamsConnecton(const int connectIdx, const QString &drvUid, QJsonObject &params)
{
    if (m_drivers)
        return m_drivers->editParamsConnecton(connectIdx, drvUid, params);
    return false;
}

Driver *AAnalyserApplication::getDriver(const QStringList &protocols, const int index) const
{
    if (m_drivers)
        return m_drivers->getDriver(protocols, index);
    return nullptr;
}

QList<DeviceProtocols::Ports> AAnalyserApplication::getDriverPorts(const QString &drvUid) const
{
    if (m_drivers)
        return m_drivers->getDriverPorts(drvUid);
    return QList<DeviceProtocols::Ports>();
}
