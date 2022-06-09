#include "aanalyserapplication.h"

#include <QStandardPaths>
#include <QTimer>
#include <QMessageBox>
#include <QLayout>
#include <QDesktopWidget>
#include <QDebug>

#include "mainwindow.h"
#include "metodicsfactory.h"
#include "driversfactory.h"
#include "factorsfactory.h"
#include "driver.h"
#include "normsmanager.h"
#include "executewidget.h"
#include "databaseresultwidget.h"
#include "signalanalysiswidget.h"
#include "summarieswidget.h"
#include "database.h"
#include "datadefines.h"
#include "exitcodes.h"
#include "settingsprovider.h"
#include "visualsfactory.h"
#include "visualdescriptor.h"
#include "log.h"
#include "addtesttosummarydialog.h"

AAnalyserApplication::AAnalyserApplication(int &argc, char **argv)
    : QApplication(argc, argv)
    , m_metodics(new MetodicsFactory(this))
    , m_drivers(new DriversFactory(this))
    , m_factors(new FactorsFactory(this))
    , m_normsManager(new NormsManager(this))
    , m_visualsFactory(new VisualsFactory(this))
{
    setApplicationName("a-analyzer");
//    setApplicationDisplayName(tr("Физиологические исследования a-analyzer")); Не переводится
    setOrganizationName("A-Med");
//    setWindowIcon(QIcon(":/images/MainIcon2.ico"));

    //! Это необходимо сделать сдесь, чтоб получить и запомнить uid приложения,
    //! чтобы использовать его дальше, в настройках и файлах
    DataDefines::appCopyUid();

    QTimer::singleShot(100, [=]()
    {
        m_database = new DataBase(this);
        connect(m_database, &DataBase::connected, this, &AAnalyserApplication::dbConnected);
        connect(m_database, &DataBase::disconnected, this, &AAnalyserApplication::dbDisconnected);
        connect(m_database, &DataBase::newTest, this, &AAnalyserApplication::newTest);

        connect(static_cast<MainWindow*>(m_mw), &MainWindow::dataBaseChange, m_database, &DataBase::changeDatabase);
        connect(static_cast<MainWindow*>(m_mw), &MainWindow::dataBaseClear, m_database, &DataBase::clear);

        connect(m_normsManager, &NormsManager::personalNormRecalculated,
                this, &AAnalyserApplication::personalNormRecalculated);
        connect(m_normsManager, &NormsManager::personalNormDeleted,
                this, &AAnalyserApplication::personalNormDeleted);
        connect(m_normsManager, &NormsManager::personalNormContainedChange,
                this, &AAnalyserApplication::personalNormContainedChange);
        connect(m_database, &DataBase::changeTestCondition,
                this, &AAnalyserApplication::changeTestCondition);
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
    if (m_factors)
        delete(m_factors);
//    if (m_addTSDlg)       TODO: Не пойму, почему вызывает сбой !!!
//        delete m_addTSDlg;
}

QMainWindow *AAnalyserApplication::mainWindow() const
{
    return m_mw;
}

void AAnalyserApplication::setMainWindow(QMainWindow *mw)
{
    m_mw = mw;
    connect(this, &AAnalyserApplication::dbConnected, static_cast<MainWindow*>(m_mw), &MainWindow::onDbConnected);
    connect(this, &AAnalyserApplication::dbDisconnected, static_cast<MainWindow*>(m_mw), &MainWindow::obDbDisconnected);
}

QRect AAnalyserApplication::getPatientWindowGeometry()
{
    if (desktop()->screenCount() == 1)
    {
        for (int i = 0; i < desktop()->screenCount(); ++i)
            if (desktop()->availableGeometry(i).contains(m_mw->geometry().center()))
                return desktop()->availableGeometry(i);
    }
    else
    {
        int n = -1;
        for (int i = 0; i < desktop()->screenCount(); ++i)
            if (desktop()->availableGeometry(i).contains(m_mw->geometry().center()))
            {
                n = i;
                break;
            }

        auto wpn = SettingsProvider::valueFromRegAppCopy("", "PatientWindowNumber", static_cast<QVariant>(1)).toInt();

        if (wpn != n && wpn >= 0 && wpn < desktop()->screenCount())
            return desktop()->availableGeometry(wpn);
        else
        if (n == 0)
            return desktop()->availableGeometry(1);
        else
            return desktop()->availableGeometry(0);
    }

    return desktop()->availableGeometry();
}

void AAnalyserApplication::showClientPage(const QString &uidPage)
{
    static_cast<MainWindow*>(m_mw)->showClientPage(uidPage);
}

DataBase *AAnalyserApplication::getDB()
{
    return m_database;
}

void AAnalyserApplication::changeDatabase(const QString &dataBaseFolder)
{
    m_database->changeDatabase(dataBaseFolder);
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

DataDefines::TestInfo AAnalyserApplication::getSelectedTest() const
{
    DataDefines::TestInfo ti;
    if (m_testUid != "" && m_database)
        m_database->getTestInfo(m_testUid, ti);
    return ti;
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

void AAnalyserApplication::getMethodicCount(const int count)
{
    emit methodicCount(count);
}

void AAnalyserApplication::doSelectBugTest(const QString &uid)
{
    //! Возможно много лишних срабатываний
    if (uid != m_testUid)
        m_testUid = uid;
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

void AAnalyserApplication::deleteTest()
{
    if (m_testUid != "")
    {
        DataDefines::TestInfo ti;
        m_database->getTestInfo(m_testUid, ti);
        DataDefines::PatientKard patient;
        m_database->getPatient(ti.patientUid, patient);
        auto mi = m_metodics->metodic(ti.metodUid);
        auto mr = QMessageBox::question(nullptr, tr("Подтверждение"), QString(tr("Удалить запись о тесте?") + "\n" +
                                                                              tr("Пациент") + ": %1\n" +
                                                                              tr("Методика") + ": %2\n" +
                                                                              tr("Дата и время проведения") + ": %3")
                                        .arg(patient.fio).arg(mi.name).arg(ti.dateTime.toString("dd.MM.yyyy hh:mm")));
        if (mr == QMessageBox::Yes)
        {
            deleteTest(m_testUid);
//            if (m_database->deleteTest(m_testUid))
//                emit removeTest(m_testUid);
            m_testUid = "";
        }
    }
    else
        QMessageBox::information(nullptr, tr("Предупрежение"), tr("Не выбран тест"));
}

void AAnalyserApplication::deleteTest(const QString &testUid)
{
    if (m_database->deleteTest(testUid))
        QTimer::singleShot(100, [=]
        {
            emit removeTest(testUid);
        });
}

void AAnalyserApplication::showDataBase()
{
    showClientPage(ClientWidgets::uidDatabaseResultWidgetUid);
}

void AAnalyserApplication::signalsAnalysis()
{
    showClientPage(ClientWidgets::uidSignalAnalysisWidgetUid);
}

void AAnalyserApplication::summaries()
{
    showClientPage(ClientWidgets::uidSummariesWidgetUid);
}

void AAnalyserApplication::summaryAddTest()
{
    if (m_testUid != "")
    {
        if (!m_addTSDlg)
        {
            m_addTSDlg = new AddTestToSummaryDialog(this->mainWindow());
            connect(m_addTSDlg, &AddTestToSummaryDialog::accepted, this, &AAnalyserApplication::on_AddTestToSummaryAccepted);
        }
        m_addTSDlg->show();
    }
    else
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Не выбран тест"));
}

void AAnalyserApplication::summaryBuild()
{

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

Driver *AAnalyserApplication::getDriverByProtocols(const QStringList &protocols, const int index) const
{
    if (m_drivers)
    {
        auto *drv = m_drivers->getDriverByProtocols(protocols, index);
        if (drv)
            connect(drv, &Driver::sendData, this, &AAnalyserApplication::sendData);
        return drv;
    }
    return nullptr;
}

Driver *AAnalyserApplication::getDriverByFormats(const QStringList &formats, const int index) const
{
    if (m_drivers)
    {
        auto *drv = m_drivers->getDriverByFormats(formats, index);
        if (drv)
            connect(drv, &Driver::sendData, this, &AAnalyserApplication::sendData);
        return drv;
    }
    return nullptr;
}

QList<DeviceProtocols::Ports> AAnalyserApplication::getDriverPorts(const QString &drvUid) const
{
    if (m_drivers)
        return m_drivers->getDriverPorts(drvUid);
    return QList<DeviceProtocols::Ports>();
}

void AAnalyserApplication::registerVisual(VisualDescriptor *visual)
{
    if (m_visualsFactory)
        m_visualsFactory->registerVisual(visual);
}

int AAnalyserApplication::visualCount(const BaseDefines::TestLevel level)
{
    if (m_visualsFactory)
        return m_visualsFactory->visualCount(level);
    return 0;
}

VisualDescriptor *AAnalyserApplication::getVisual(const BaseDefines::TestLevel level, const int idx)
{
    if (m_visualsFactory)
        return m_visualsFactory->getVisual(level, idx);
    return nullptr;
}

FactorsDefines::FactorInfo AAnalyserApplication::getFactorInfo(const QString &uid) const
{
    if (m_factors)
        return m_factors->getFactorInfo(uid);
    return FactorsDefines::FactorInfo();
}

QString AAnalyserApplication::getGroupName(const QString &groupUid) const
{
    if (m_factors)
        return m_factors->getGroupName(groupUid);
    return QString("");
}

void AAnalyserApplication::registerFactor(const QString &uid, const QString &groupUid,
                                          const QString &name, const QString &shortName,
                                          const QString &measure, const int format,
                                          const double multiplier, const FactorsDefines::NormSide side, const int agingPeriod)
{
    if (m_factors)
        m_factors->registerFactor(uid, groupUid, name, shortName, measure, format, multiplier, side, agingPeriod);
}

void AAnalyserApplication::registerGroup(const QString &uid, const QString &name)
{
    if (m_factors)
        m_factors->registerGroup(uid, name);
}

void AAnalyserApplication::registerGroup(MultiFactorDescriptor *group)
{
    if (m_factors)
        m_factors->registerGroup(group);
}

int AAnalyserApplication::multiFactorCount(const BaseDefines::TestLevel level)
{
    if (m_factors)
        return m_factors->multiFactorCount(level);
    return 0;
}

MultiFactorDescriptor *AAnalyserApplication::getMultiFactor(const BaseDefines::TestLevel level, const int idx)
{
    if (m_factors)
        return m_factors->getMultiFactor(level, idx);
    return nullptr;
}

QStringList AAnalyserApplication::getTestConditions()
{
    if (m_normsManager)
        return m_normsManager->getTestConditions();
    return QStringList();
}

bool AAnalyserApplication::getTestConditionInfo(const QString &uid, DataDefines::TestConditionInfo &ci)
{
    if (m_normsManager)
        return m_normsManager->getTestConditionInfo(uid, ci);
    return false;
}

bool AAnalyserApplication::getPersonalNorm(const QString &testUid, QList<DataDefines::PersonalNormInfo> &pnil) const
{
    if (m_normsManager)
        return m_normsManager->getPersonalNorm(testUid, pnil);
    return false;
}

int AAnalyserApplication::getPersonalNormContainedTestCount(const QString &testUid) const
{
    if (m_normsManager)
        return m_normsManager->getPersonalNormContainedTestCount(testUid);
    return 0;
}

void AAnalyserApplication::setTestNormContained(const QString &testUid, const bool isNormContained) const
{
    if (m_normsManager)
        m_normsManager->setTestNormContained(testUid, isNormContained);
}

void AAnalyserApplication::calculatePersonalNorm(const QString &patientUid, const QString &methodUid, const QString &conditionUid) const
{
    if (m_normsManager)
        m_normsManager->calculate(patientUid, methodUid, conditionUid);
}

void AAnalyserApplication::calculateAllPersonalNorms()
{
    if (m_normsManager)
        m_normsManager->calculateAll();
}

bool AAnalyserApplication::getGroupNorms(const QString &methodicUid, const QString &conditionUid,
                                         QList<DataDefines::GroupNormInfo> &gni)
{
    if (m_normsManager)
        return m_normsManager->getGroupNorms(methodicUid, conditionUid, gni);
    return false;
}

void AAnalyserApplication::setLanguargeCode(const QString &code)
{
    m_languargeCode = code;
}

QString AAnalyserApplication::languargeCode() const
{
    return m_languargeCode;
}

void AAnalyserApplication::changeApplicationParam(const QString &group, const QString &param, const QVariant &value)
{
    emit applicationParamChanged(group, param, value);
}

bool AAnalyserApplication::notify(QObject *re, QEvent *ev)
{
    try
    {
        return QApplication::notify(re, ev);
    }
    catch(QString &e)
    {
        QString s = EXIT_COMMENTS[EC_MAINLOOP_ERROR].arg(e).arg(re->objectName());
        qDebug() << s;
        log(s);
        QTimer::singleShot(0, this, [this]()
        {
            exit(EC_MAINLOOP_ERROR);
        });
    }
    catch(...)
    {
        QString s = EXIT_COMMENTS[EC_MAINLOOP_UNKNOWN_ERROR].arg(re->objectName());
        qDebug() << s;
        log(s);
        QTimer::singleShot(0, this, [this]()
        {
            exit(EC_MAINLOOP_UNKNOWN_ERROR);
        });
    }
    return false;
}

void AAnalyserApplication::on_AddTestToSummaryAccepted()
{
    if (m_addTSDlg->mode() == SummaryDefines::atmExists)
    {
        if (m_addTSDlg->summary() == "")
            QMessageBox::warning(nullptr, tr("Предупреждение"), tr("Сводка не выбрана"));
        else
            emit addTestToSummary(m_testUid, m_addTSDlg->mode(), m_addTSDlg->summary(), m_addTSDlg->kind());
    }
    else
    if (m_addTSDlg->mode() == SummaryDefines::atmNew)
    {
        if (m_addTSDlg->summaryName() == "")
            QMessageBox::warning(nullptr, tr("Предупреждение"), tr("Не задано название сводки"));
        else
            emit addTestToSummary(m_testUid, m_addTSDlg->mode(), m_addTSDlg->summaryName(), m_addTSDlg->kind());
    }
    if (m_addTSDlg->mode() == SummaryDefines::atmActive)
        emit addTestToSummary(m_testUid, m_addTSDlg->mode(), "", m_addTSDlg->kind());
}

