#include "aanalyserapplication.h"

#include <QStandardPaths>
#include <QTimer>
#include <QMessageBox>
#include <QLayout>
#include <QDesktopWidget>
#include <QUuid>
#include <QDebug>

#include "mainwindow.h"
#include "metodicsfactory.h"
#include "driversfactory.h"
#include "factorsfactory.h"
#include "signaltransformfactory.h"
#include "driver.h"
#include "normsmanager.h"
#include "executewidget.h"
#include "databaseresultwidget.h"
#include "signalanalysiswidget.h"
#include "summarieswidget.h"
#include "personalprogramwidget.h"
#include "database.h"
#include "datadefines.h"
#include "exitcodes.h"
#include "settingsprovider.h"
#include "visualsfactory.h"
#include "visualdescriptor.h"
#include "log.h"
#include "addtesttosummarydialog.h"
#include "dataprovider.h"
#include "signaltransformerparamswidget.h"
#include "personalprogrammanager.h"
#include "selectpersonalprogramdialog.h"
#include "openpersonalprogramdialog.h"
#include "patientsmodel.h"
#include "patientsproxymodel.h"
#include "aanalysersettings.h"
#include "personalprogram.h"

AAnalyserApplication::AAnalyserApplication(int &argc, char **argv)
    : QApplication(argc, argv)
    , m_metodics(new MetodicsFactory(this))
    , m_drivers(new DriversFactory(this))
    , m_factors(new FactorsFactory(this))
    , m_transformers(new SignalTransformFactory(this))
    , m_normsManager(new NormsManager(this))
    , m_visualsFactory(new VisualsFactory(this))
    , m_ppManager(new PersonalProgramManager(this))
    , m_mdlPatients(new PatientsModel(this))
    , m_pmdlPatients(new PatientsProxyModel(this))
    , m_pmdlPatientsPP(new PatientsProxyModel(this))
{
    setApplicationName("a-analyzer");
//    setApplicationVersion("1.0");
//    setApplicationDisplayName(tr("Исследования А-Мед")); Не переводится
    setOrganizationName("A-Med");
//    setWindowIcon(QIcon(":/images/MainIcon2.ico"));

    //! Это необходимо сделать сдесь, чтоб получить и запомнить uid приложения,
    //! чтобы использовать его дальше, в настройках и файлах
    DataDefines::appCopyUid();

    QTimer::singleShot(100, [=]()
    {
        m_database = new DataBase(this);
        connect(m_database, &DataBase::connected, this, &AAnalyserApplication::on_dbConnected);
        connect(m_database, &DataBase::connected, this, &AAnalyserApplication::dbConnected);
        connect(m_database, &DataBase::disconnected, this, &AAnalyserApplication::dbDisconnected);
        connect(m_database, &DataBase::newTest, this, &AAnalyserApplication::newTest);
        connect(m_database, &DataBase::sectionCreated, this, &AAnalyserApplication::sectionCreated);

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

        m_pmdlPatients->setSourceModel(m_mdlPatients);
        m_pmdlPatientsPP->setSourceModel(m_mdlPatients);

        auto rm = SettingsProvider::valueFromRegAppCopy("", AAnalyserSettingsParams::pn_runningMode, BaseDefines::rmOperator).toInt();
        m_runningMode = static_cast<BaseDefines::RunningMode>(rm);
    });

    m_asi.uidMethodic = "";
    m_asi.kind = SummaryDefines::skNone;

    m_openedTests.clear();
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
    if (m_transformers)
        delete m_transformers;
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

bool AAnalyserApplication::restoreClientPage()
{
    return static_cast<MainWindow*>(m_mw)->restoreClientPage();
}

DataBase *AAnalyserApplication::getDB()
{
    return m_database;
}

void AAnalyserApplication::changeDatabase(const QString &dataBaseFolder)
{
    m_database->changeDatabase(dataBaseFolder);
}

DataDefines::PatientKard AAnalyserApplication::getCurrentPatient() const
{
    DataDefines::PatientKard kard;
    if (m_patientUid != "" && m_database)
        m_database->getPatient(m_patientUid, kard);
    return kard;
}

MetodicDefines::MetodicInfo AAnalyserApplication::getCurrentMetodic() const
{
    if (m_metodics && m_metodicUid != "")
        return m_metodics->metodic(m_metodicUid);
    return MetodicDefines::MetodicInfo();
}

DataDefines::TestInfo AAnalyserApplication::getCurrentTest() const
{
    DataDefines::TestInfo ti;
    if (m_testUid != "" && m_database)
        m_database->getTestInfo(m_testUid, ti);
    return ti;
}

int AAnalyserApplication::selectedTestsCount() const
{
    return m_selectedTests.size();
}

QString AAnalyserApplication::selectedTest(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < m_selectedTests.size());
    return m_selectedTests.at(idx);
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

void AAnalyserApplication::setSelectedTests(QStringList &tests)
{
    m_selectedTests.clear();
    m_selectedTests << tests;
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

void AAnalyserApplication::showPersonalPrograms()
{
    showClientPage(ClientWidgets::uidPersonalProgramWidgetUid);
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
//    if (m_testUid != "")
    if (selectedTestsCount() > 0)
    {
        if (!m_addTSDlg)
        {
            m_addTSDlg = new AddTestToSummaryDialog(this->mainWindow());
            connect(m_addTSDlg, &AddTestToSummaryDialog::accepted, this, &AAnalyserApplication::on_AddTestToSummaryAccepted);
        }
        m_addTSDlg->show();
    }
    else
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Не выбран ни один тест"));
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

void AAnalyserApplication::readDailyProgramList(QStandardItemModel &model, QStringList uids)
{
    if (m_ppManager)
        m_ppManager->readDailyProgramList(model, uids);
}

void AAnalyserApplication::saveDailyProgramList(const QStandardItemModel &model)
{
    if (m_ppManager)
        m_ppManager->saveDailyProgramList(model);
}

void AAnalyserApplication::readPersonalProgramList(QStandardItemModel &model)
{
    if (m_ppManager)
        m_ppManager->readPersonalProgramList(model);
}

void AAnalyserApplication::savePersonalProgramList(const QStandardItemModel &model)
{
    if (m_ppManager)
        m_ppManager->savePersonalProgramList(model);
}

void AAnalyserApplication::assignDailyProgramsForPersonal(QString &uidPP, QStringList &uidDPs)
{
    if (m_ppManager)
        m_ppManager->assignDailyProgramsForPersonal(uidPP, uidDPs);
}

void AAnalyserApplication::editDailyProgramsForPersonal(QString &uidPP, QStringList &uidDPs)
{
    if (m_ppManager)
        m_ppManager->editDailyProgramsForPersonal(uidPP, uidDPs);
}

void AAnalyserApplication::clearListDailyProgramsForPersonal(QString &uidPP)
{
    if (m_ppManager)
        m_ppManager->clearListDailyProgramsForPersonal(uidPP);
}

QStringList AAnalyserApplication::getListDailyProgramsForPersonal(QString &uidPP)
{
    if (m_ppManager)
        return m_ppManager->getListDailyProgramsForPersonal(uidPP);
    return QStringList();
}

bool AAnalyserApplication::assignPPForPatient()
{
    auto pi = getCurrentPatient();
    if (pi.uid != "")
    {
        if (DataProvider::getActivePersonalProgramForPatient(pi.uid) == QJsonObject())
        {
            SelectPersonalProgramDialog dlg(nullptr);
            if (dlg.exec() == QDialog::Accepted)
            {
                QString uidPPAssigned = "";
                auto ppObj = m_ppManager->assignPersonalProgramForPatient(getCurrentPatient().uid, dlg.personalProgramUid(), uidPPAssigned);
                DataProvider::assignPersonalProgramForPatient(uidPPAssigned, ppObj);

                //! Сообщение о назначении индивидуальной программы пациенту с запросом на открытие
                OpenPersonalProgramDialog dlgOpen(nullptr);
                dlgOpen.setPatientFio(pi.fio);
                auto ppName = ppObj["pp"].toObject()["name"].toString();
                auto ppPic = QPixmap(ppObj["pp"].toObject()["logo_file_name"].toString());
                dlgOpen.setPersonalProgram(ppName, ppPic);

                //! Извещаем мир о назначении индивидуальной программы для пациента
                emit assignedPPForPatient(pi.uid, uidPPAssigned);

                //! Открытие индивидуальной программы
                if (dlgOpen.exec() == QDialog::Accepted)
                {
                    showClientPage(ClientWidgets::uidPersonalProgramWidgetUid);
                    SettingsProvider::setValueToRegAppCopy("MainWindow", "MainClientWidget", ClientWidgets::uidPersonalProgramWidgetUid);
                    doSelectPatient(pi.uid);
                }

                return true;
            }
        }
        else
            QMessageBox::information(nullptr,
                                     tr("Предупреждение"),
                                     tr("Для пациента уже назначена индивидуальная программа")+
                                     "\n" + tr("Пациент") + ": " + pi.fio );
    }
    else
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Не выбран пациент"));

    return false;
}

bool AAnalyserApplication::cancelPPForPatient()
{
    auto pi = getCurrentPatient();
    if (pi.uid != "")
    {
        auto pp = DataProvider::getActivePersonalProgramForPatient(pi.uid);
        if (pp != QJsonObject())
        {
            auto mr = QMessageBox::question(nullptr,
                                            tr("Запрос"),
                                            tr("Отменить индивидуальную программу для пациента?") +
                                               "\n" + tr("Пациент") + ": " + pi.fio);
            if (mr == QMessageBox::Yes)
            {
                //! Если не проведены тесты, то удалить программу из БД, иначе пометить ее неактивной
                if (PersonalProgram::isTestByPPExists(pp))
                    DataProvider::deactivatePersonalProgramForPatient(pi.pp_uid);
                else
                    DataProvider::deletePersonalProgramForPatient(pi.pp_uid);

                //! Удалить программу у пациента
                pi.pp_uid = "";
                DataProvider::updatePatient(pi);

                //! Извещаем мир об отмене индивидуальной программы для пациента
                emit canceledPPForPatient(pi.uid);
            }
        }
        else
            QMessageBox::information(nullptr,
                                     tr("Предупреждение"),
                                     tr("Для пациента не назначена индивидуальная программа") + " - " + pi.fio);
    }
    else
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Не выбран пациент"));

    return false;
}

void AAnalyserApplication::doChannelChanged(const QString &probeUid, const QString &channelId)
{
    //! Нам надо только оповестить мир об изменении, чтоб мир перестроился, если ему нужно
    emit channelChanged(probeUid, channelId);
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

void AAnalyserApplication::registerSignalTransformer(SignalTransformer *st)
{
    if (m_transformers)
        m_transformers->registerTransformer(st);
}

int AAnalyserApplication::signalTransformersCount()
{
    if (m_transformers)
        return m_transformers->count();
    return 0;
}

QString AAnalyserApplication::signalTransformerUid(const int idx) const
{
    if (m_transformers)
        return m_transformers->uid(idx);
    return QUuid().toString();
}

QString AAnalyserApplication::signalTransformerName(const int idx) const
{
    if (m_transformers)
        return m_transformers->name(idx);
    return "";
}

void AAnalyserApplication::transformSignal(const int idx, QVector<double> &buffer, const QJsonObject& params) const
{
    if (m_transformers)
        m_transformers->transform(idx, buffer, params);
}

void AAnalyserApplication::transformSignal(const QString uid, QVector<double> &buffer, const QJsonObject& params) const
{
    if (m_transformers)
        m_transformers->transform(uid, buffer, params);
}

SignalTransformerParamsWidget *AAnalyserApplication::getSignalTransformParamsWidget(const int idx) const
{
    if (m_transformers)
        return m_transformers->getParamsWidget(idx);
    return nullptr;
}

SignalTransformerParamsWidget *AAnalyserApplication::getSignalTransformParamsWidget(const QString uid) const
{
    if (m_transformers)
        return m_transformers->getParamsWidget(uid);
    return nullptr;
}

void AAnalyserApplication::rememberTransformerScheme(const QJsonArray &sheme)
{
    if (m_transformers)
        m_transformers->rememberScheme(sheme);
}

QJsonArray AAnalyserApplication::getTransformerScheme()
{
    if (m_transformers)
        return m_transformers->getScheme();
    return QJsonArray();
}

void AAnalyserApplication::clearTransformerSheme()
{
    if (m_transformers)
        m_transformers->clearSheme();
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

void AAnalyserApplication::setActiveSummary(const QString &uidMethodic, const SummaryDefines::Kind &kind)
{
    m_asi.uidMethodic = uidMethodic;
    m_asi.kind = kind;
}

void AAnalyserApplication::getActiveSummary(SummaryDefines::ActiveSummaryInfo &asi)
{
    asi.uidMethodic = m_asi.uidMethodic;
    asi.kind = m_asi.kind;
}

void AAnalyserApplication::initProgress(const QString &title, const int begin, const int end, const QString &stage)
{
    if (m_mw)
        static_cast<MainWindow*>(m_mw)->initProgress(title, begin, end, stage);
}

void AAnalyserApplication::setProgressPosition(const int pos, const QString &stage)
{
    if (m_mw)
        static_cast<MainWindow*>(m_mw)->setProgressPosition(pos, stage);
}

void AAnalyserApplication::doneProgress()
{
    if (m_mw)
        static_cast<MainWindow*>(m_mw)->doneProgress();
}

void AAnalyserApplication::addOpenedTest(const QString &testUid, QWidget *visual)
{
    m_openedTests.insert(testUid, visual);
}

void AAnalyserApplication::delOpenedTestFromList(const QString &testUid)
{
    if (m_openedTests.contains(testUid))
        m_openedTests.remove(testUid);
}

void AAnalyserApplication::clearOpenedTestList()
{
    m_openedTests.clear();
}

QWidget *AAnalyserApplication::getOpenedTest(const QString &testUid)
{
    if (m_openedTests.contains(testUid))
        return m_openedTests.value(testUid, nullptr);
    return nullptr;
}

void AAnalyserApplication::setRunningMode()
{
    auto rm = SettingsProvider::valueFromRegAppCopy("", AAnalyserSettingsParams::pn_runningMode, BaseDefines::rmOperator).toInt();
    m_runningMode = static_cast<BaseDefines::RunningMode>(rm);
}

void AAnalyserApplication::doSettingsChanged()
{
    setRunningMode();
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

void AAnalyserApplication::on_dbConnected()
{
    if (m_mdlPatients)
        m_mdlPatients->load();
    m_pmdlPatientsPP->setShowPPMode(PatientsProxyModel::sppActive);
    m_ppManager->on_dbConnected();
    qDebug() << Q_FUNC_INFO;

}

void AAnalyserApplication::on_AddTestToSummaryAccepted()
{
    auto testInfo = [&](const QString testUid, QString& info)
    {
        DataDefines::TestInfo ti;
        DataProvider::getTestInfo(testUid, ti);
        DataDefines::PatientKard kard;
        DataProvider::getPatient(ti.patientUid, kard);
        info = kard.fio + " - " + ti.dateTime.toString("dd.MM.yyyy hh:mm");
    };

    if (m_addTSDlg->mode() == SummaryDefines::atmNew)
    {
        if (m_addTSDlg->summaryName() == "")
            QMessageBox::information(nullptr, tr("Предупреждение"), tr("Не задано название сводки"));
        else
        {
            if (isOneMethodicOnAddTests())
            {
                initProgress("Добавление показателей в сводку", 0, selectedTestsCount());
                emit addTestToSummaryBegin();
                for (int i = 0; i < selectedTestsCount(); ++i)
                {
                    auto testUid = selectedTest(i);
                    QString info = "";
                    testInfo(testUid, info);
                    setProgressPosition(i, info);
                    if (i == 0)
                        emit addTestToSummary(testUid, SummaryDefines::atmNew, m_addTSDlg->summaryName(), m_addTSDlg->kind());
                    else
                        emit addTestToSummary(testUid, SummaryDefines::atmActive, "", m_addTSDlg->kind());
                }
                doneProgress();
                emit addTestToSummaryEnd();
            }
            else
                QMessageBox::information(nullptr, tr("Предупреждение"), tr("Выбраны тесты по разным методикам"));
        }
    }
    if (m_addTSDlg->mode() == SummaryDefines::atmActive)
    {
        initProgress("Добавление показателей в сводку", 0, selectedTestsCount());
        emit addTestToSummaryBegin();
        for (int i = 0; i < selectedTestsCount(); ++i)
        {
            auto testUid = selectedTest(i);
            QString info = "";
            testInfo(testUid, info);
            setProgressPosition(i, info);
            emit addTestToSummary(testUid, m_addTSDlg->mode(), "", m_addTSDlg->kind());
        }
        doneProgress();
        emit addTestToSummaryEnd();
    }

}

bool AAnalyserApplication::isOneMethodicOnAddTests()
{
    bool retval = true;
    QString testUid = "";
    if (selectedTestsCount() > 0)
    {
        testUid = selectedTest(0);
        DataDefines::TestInfo ti;
        DataProvider::getTestInfo(testUid, ti);
        testUid = ti.metodUid;
    }

    for (int i = 1; i < selectedTestsCount(); ++i)
    {
        DataDefines::TestInfo ti;
        DataProvider::getTestInfo(selectedTest(i), ti);
        auto mUid = ti.metodUid;
        if (mUid != testUid)
        {
            retval = false;
            break;
        }
    }
    return retval;
}

//void AAnalyserApplication::on_AddTestToSummaryAccepted()
//{
//    if (m_addTSDlg->mode() == SummaryDefines::atmNew)
//    {
//        if (m_addTSDlg->summaryName() == "")
//            QMessageBox::warning(nullptr, tr("Предупреждение"), tr("Не задано название сводки"));
//        else
//            emit addTestToSummary(m_testUid, m_addTSDlg->mode(), m_addTSDlg->summaryName(), m_addTSDlg->kind());
//    }
//    if (m_addTSDlg->mode() == SummaryDefines::atmActive)
//        emit addTestToSummary(m_testUid, m_addTSDlg->mode(), "", m_addTSDlg->kind());
//}

