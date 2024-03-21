#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "aanalyserapplication.h"
#include "databaseresultwidget.h"
#include "personalprogramwidget.h"
#include "executewidget.h"
#include "signalanalysiswidget.h"
#include "summarieswidget.h"
#include "clientwidget.h"
#include "devicecontroldialog.h"
#include "datadefines.h"
#include "settingsprovider.h"
#include "databasepropertydialog.h"
#include "dataprovider.h"
#include "aanalysersettings.h"
#include "aanalyserdefines.h"
#include "aanalyserbuild.h"
#include "aboutdialog.h"
#include "dailyprogramseditor.h"
#include "personalprogramseditor.h"

#include <QFile>
#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QAction>
#include <QDebug>

namespace
{
    /*!
     * \brief Идентификаторы цветовых схем ColorShemesId enum
     */
    enum ColorShemesId
    {
          csiMain = 0
        , csiGreen
        , csiYellow
        , csiRed
    };

    /*!
     * \brief Перечень цветовых схем ColorShemesList
     */
    QList<ColorShemesId> ColorShemesList = {csiMain, csiGreen, csiYellow, csiRed};

    /*!
     * \brief Названия цветовых схем ColorShemesName
     */
    static QMap<ColorShemesId, QString> ColorShemesName =
    {
          std::pair<ColorShemesId, QString> (csiMain, QCoreApplication::tr("Основная"))
        , std::pair<ColorShemesId, QString> (csiGreen, QCoreApplication::tr("Зеленая"))
        , std::pair<ColorShemesId, QString> (csiYellow, QCoreApplication::tr("Желтая"))
        , std::pair<ColorShemesId, QString> (csiRed, QCoreApplication::tr("Красная"))
    };

    /*!
     * \brief Названия файлов цветовых схем ColorShemesFile
     */
    static QMap<ColorShemesId, QString> ColorShemesFile =
    {
          std::pair<ColorShemesId, QString> (csiMain, ":/qss/main.qss")
        , std::pair<ColorShemesId, QString> (csiGreen, ":/qss/main_green.qss")
        , std::pair<ColorShemesId, QString> (csiYellow, ":/qss/main_yellow.qss")
        , std::pair<ColorShemesId, QString> (csiRed, ":/qss/main_red.qss")
    };
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(AAnalyserDefines::MainIcon));
    setWindowTitle(AAnalyserDefines::MainWindowTitle);
    createClientWidgets();

    m_curColorSheme = new SettingsValue("", "ColorSheme", AAnalyserDefines::MainStyleSheetFile);
    initUi(m_curColorSheme->value().toString());
    initMenu();

    auto val = SettingsProvider::valueFromRegAppCopy("MainWindow", "Geometry").toByteArray();
    restoreGeometry(val);

    ui->frProgress->setVisible(false);
    ui->wgtMode->setVisible(false);

    connect(ui->acSettings, &QAction::triggered, this, &MainWindow::onSettings);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showClientPage(const QString &uidPage)
{
    //! Запомнить показываемую страницу
    if (uidPage != m_currentClientPage)
        m_lastPages << uidPage;

    foreach (auto *wgt, m_clientWidgets)
        if (static_cast<ClientWidget*>(wgt)->uid() == m_currentClientPage)
            static_cast<ClientWidget*>(wgt)->onHide();

    foreach (auto *wgt, m_clientWidgets)
    {
        wgt->setVisible(static_cast<ClientWidget*>(wgt)->uid() == uidPage);

        if (static_cast<ClientWidget*>(wgt)->uid() == uidPage)
        {
            m_currentClientPage = uidPage;
            static_cast<ClientWidget*>(wgt)->onShow();
            auto mv = static_cast<ClientWidget*>(wgt)->isExternalControl();
            ui->menuBar->setVisible(mv);

            if (mv)
                setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint | Qt::WindowFullscreenButtonHint | Qt::WindowMinMaxButtonsHint);
            else
                setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint | Qt::WindowFullscreenButtonHint);
            show();

            if (uidPage == ClientWidgets::uidDatabaseResultWidgetUid)
                ui->acViewDataBase->setChecked(true);
            else
            if (uidPage == ClientWidgets::uidPersonalProgramWidgetUid)
                ui->acViewPersonalProgram->setChecked(true);
        }
    }

    ui->acViewDataBase->setEnabled(uidPage == ClientWidgets::uidPersonalProgramWidgetUid);
    ui->acViewPersonalProgram->setEnabled(uidPage == ClientWidgets::uidDatabaseResultWidgetUid);
}

bool MainWindow::restoreClientPage()
{
    if (m_lastPages.count() > 1)
    {
        auto pageId = m_lastPages.pop();  //! Это текущая, которая должна быть закрыта
        pageId = m_lastPages.pop();       //! А вот эта предыдущая, которую надо показать
        showClientPage(pageId);           //! Эта функция не только ее покажет, но и запомнит ее снова
        return true;
    }
    return false;
}

QString MainWindow::currentClientPage()
{
    if (m_lastPages.count() > 0)
        return m_lastPages.top();
    return QUuid().toString();
}

QWidget *MainWindow::getExecuteWidget()
{
    foreach (auto *wgt, m_clientWidgets)
        if (static_cast<ClientWidget*>(wgt)->uid() == ClientWidgets::uidExecuteWidgetUid)
            return wgt;
    return nullptr;
}

void MainWindow::initProgress(const QString &title, const int begin, const int end, const QString &stage)
{
    ui->lblProgressTitle->setText(title);
    ui->lblProgressStage->setText(stage);
    ui->progressBar->setMinimum(begin);
    ui->progressBar->setMaximum(end);
    ui->frProgress->setVisible(true);
}

void MainWindow::setProgressPosition(const int pos, const QString &stage)
{
    ui->progressBar->setValue(pos);
    ui->lblProgressStage->setText(stage);
}

void MainWindow::doneProgress()
{
    ui->frProgress->setVisible(false);
    ui->progressBar->setValue(0);
    ui->lblProgressTitle->setText("");
    ui->lblProgressStage->setText("");
}

void MainWindow::onDbConnected()
{
    foreach (auto wgt, m_clientWidgets)
        static_cast<ClientWidget*>(wgt)->onDBConnect();
}

void MainWindow::obDbDisconnected()
{
    foreach (auto wgt, m_clientWidgets)
        static_cast<ClientWidget*>(wgt)->onDBDisconnect();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (m_currentClientPage == ClientWidgets::uidExecuteWidgetUid)
    {
        auto mr = QMessageBox::question(this, tr("Предупреждение"), tr("Вы уверены, что хотите прервать проведение теста?"),
                                        QMessageBox::Yes | QMessageBox::No);
        if (mr == QMessageBox::Yes)
            restoreClientPage();
        event->ignore();
    }
    else
    if (m_currentClientPage == ClientWidgets::uidSignalAnalysisWidgetUid)
    {
        restoreClientPage();
        event->ignore();
    }
    else
    if (m_currentClientPage == ClientWidgets::uidSummariesWidgetUid)
    {
        restoreClientPage();
        event->ignore();
    }
    else
    {
        foreach (auto *wgt, m_clientWidgets)
        {
            auto ce = static_cast<ClientWidget*>(wgt)->isCloseProgramQuery();
            if (!ce)
            {
                event->ignore();
                return;
            }
        }

        SettingsProvider::setValueToRegAppCopy("MainWindow", "Geometry", saveGeometry());
        event->accept();
    }
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    qDebug() << Q_FUNC_INFO;
}

void MainWindow::onDeviceControl()
{
    DeviceControlDialog dlg(this);
    dlg.exec();
}

void MainWindow::onSettings()
{
    AAnalyserSettings dlg(this);
    if (dlg.exec() == QDialog::Accepted)
        static_cast<AAnalyserApplication*>(QApplication::instance())->doSettingsChanged();
}

void MainWindow::onAbout()
{
    AboutDialog dlg(this);
    dlg.exec();
}

void MainWindow::onDataBaseProperty()
{
    DataBasePropertyDialog dlg(this);
    QString path = SettingsProvider::valueFromRegAppCopy("Database", "path").toString();
    dlg.setPath(path);
    DataDefines::DatabaseInfo info = DataDefines::getDatabaseInfo(path);
    dlg.setComment(info.comment);
    dlg.setVersion(info.version);
    if (dlg.exec() == QDialog::Accepted)
    {
        DataDefines::setDatabaseComment(path, dlg.comment());
    }
}

void MainWindow::onDataBaseExport()
{
    QString path = DataDefines::aanalyserDocumentsPath();
    auto fileName = QFileDialog::getSaveFileName(this, tr("Файл для экспорта БД"), path, tr("Экспортные файлы БД (*.abde)"));
    if (fileName != "")
    {
        DataProvider::exportBD(fileName);
    }
}

void MainWindow::onDataBaseImport()
{
    QString path = DataDefines::aanalyserDocumentsPath();
    auto fileName = QFileDialog::getOpenFileName(this, tr("Файл для импорта БД"), path, tr("Экспортные файлы БД (*.abde)"));
    if (fileName != "")
    {
        DataProvider::importBD(fileName);
        initSelectDatabaseMenu();
    }
}

void MainWindow::onDataBaseSelect()
{

}

void MainWindow::onDataBaseClear()
{
    if (QMessageBox::question(this, tr("Запрос"), tr("Удалить всех пациентов со всеми обследованиями?")) ==
            QMessageBox::Yes)
        DataProvider::clear();
}

void MainWindow::onDataBaseDelTests()
{
    if (QMessageBox::question(this, tr("Запрос"), tr("Удалить все обследования?")) ==
            QMessageBox::Yes)
        DataProvider::deleteTests();
}

void MainWindow::onDataBaseCreate()
{
    if (QMessageBox::question(this, tr("Запрос"), tr("Создать новую базу данных?")) ==
            QMessageBox::Yes)
    {
        emit dataBaseCreate();
        initSelectDatabaseMenu();
    }
}

void MainWindow::on_selectDatabase()
{
    QString path = DataDefines::dataBasesPath() + static_cast<QAction*>(sender())->data().toString() + "/";
    emit dataBaseChange(path);
    initSelectDatabaseMenu();
}

void MainWindow::on_selectColorSheme()
{
    if (sender())
    {
        auto* action = static_cast<QAction*>(sender());
        action->setChecked(true);
        initUi(action->data().toString());
        m_curColorSheme->set(action->data());
    }
}

void MainWindow::on_viewDataBase()
{
    showClientPage(ClientWidgets::uidDatabaseResultWidgetUid);
    SettingsProvider::setValueToRegAppCopy("MainWindow", "MainClientWidget", ClientWidgets::uidDatabaseResultWidgetUid);
}

void MainWindow::on_viewPersonalProgram()
{
    showClientPage(ClientWidgets::uidPersonalProgramWidgetUid);
    SettingsProvider::setValueToRegAppCopy("MainWindow", "MainClientWidget", ClientWidgets::uidPersonalProgramWidgetUid);
}

void MainWindow::onSummariesShow()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->summaries();
}

void MainWindow::onAddTestToSymmary()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->summaryAddTest();
}

void MainWindow::onSummaryBuild()
{
    qDebug() << "построение сводки";
}

void MainWindow::onSummariesBrowse()
{
    qDebug() << "обзор сводок";
}

void MainWindow::on_editPersonalPrograms()
{
    PersonalProgramsEditor dlg(this);
    dlg.exec();
}

void MainWindow::on_editDailyPrograms()
{
    DailyProgramsEditor dlg(this);
    dlg.exec();
}

void MainWindow::on_PPAssign()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->assignPPForPatient();
}

void MainWindow::on_PPCancel()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->cancelPPForPatient();
}

void MainWindow::on_clearGeometry()
{
    auto mr = QMessageBox::question(nullptr, tr("Запрос"), tr("Удалить данные о расположении и размере окон, а также о расположении разделителей?"));
    if (QMessageBox::Yes == mr)
        SettingsProvider::clearGroup("Geometry");
}

void MainWindow::initUi(const QString& colorSheme)
{
    QFile style(colorSheme);
    style.open( QFile::ReadOnly );
    QString stlDetail(style.readAll() );
    setStyleSheet(stlDetail);
}

void MainWindow::initMenu()
{
    QMenu *menuDatabase = menuBar()->addMenu(tr("База данных"));
    if (AAnalyserBuild::isPPEnabled())
    {
        menuDatabase->addAction(ui->acViewDataBase);
        menuDatabase->addSeparator();
    }
    menuDatabase->addAction(ui->acDataBaseProperty);
    initSelectDatabaseMenu();
    menuDatabase->addMenu(&m_menuSelectDatabase);
    menuDatabase->addSeparator();

    menuDatabase->addAction(ui->acDataBaseExport);
    menuDatabase->addAction(ui->acDataBaseImport);

    QMenu *menuDBClear = menuDatabase->addMenu(tr("Очистить"));
    menuDBClear->addAction(ui->acDataBaseClear);
    menuDBClear->addAction(ui->acDataBaseDelTests);

    menuDatabase->addAction(ui->acDataBaseCreate);
    menuDatabase->addSeparator();

    menuDatabase->addAction(ui->acExit);



    if (AAnalyserBuild::isPPEnabled())
    {
        QMenu *menuPP = menuBar()->addMenu(tr("Индивидуальные программы"));
        menuPP->addAction(ui->acViewPersonalProgram);
        menuPP->addSeparator();
        menuPP->addAction(ui->acPPAssign);
        menuPP->addAction(ui->acPPCancel);
    }

    //--------------------------------------------------------
//    if (AAnalyserBuild::isPPEnabled())
//    {
//        QMenu *menuView = menuBar()->addMenu(tr("Вид"));
//        m_agViewsMain = new QActionGroup(this);
//        menuView->addAction(ui->acViewDataBase);
//        m_agViewsMain->addAction(ui->acViewDataBase);
//        menuView->addAction(ui->acViewPersonalProgram);
//        m_agViewsMain->addAction(ui->acViewPersonalProgram);
//        ui->acViewDataBase->setCheckable(true);
//        ui->acViewPersonalProgram->setCheckable(true);
//    }

//    auto cwv = SettingsProvider::valueFromRegAppCopy("MainWindow", "MainClientWidget", ClientWidgets::uidDatabaseResultWidgetUid).toString();
//    if (!AAnalyserBuild::isPPEnabled() && cwv == ClientWidgets::uidPersonalProgramWidgetUid)
//        cwv = ClientWidgets::uidDatabaseResultWidgetUid;

//    if (cwv == ClientWidgets::uidDatabaseResultWidgetUid)
//        ui->acViewDataBase->setChecked(true);
//    else
//    if (cwv == ClientWidgets::uidPersonalProgramWidgetUid)
//        ui->acViewPersonalProgram->setChecked(true);
//--------------------------------------------------------

//    QMenu *menuSummaries = menuBar()->addMenu(tr("Сводки"));
//    ui->acSummariesShow->setIcon(QIcon(":/images/Summaries.png"));
//    menuSummaries->addAction(ui->acSummariesShow);
//    ui->acAddTestToSummary->setIcon(QIcon(":/images/SummaryAddTest.png"));
//    menuSummaries->addAction(ui->acAddTestToSummary);
////    ui->acSummaryBuild->setIcon(QIcon(":/images/SummaryBuild.png"));
////    menuSummaries->addAction(ui->acSummaryBuild);
////    menuSummaries->addSeparator();
////    menuSummaries->addAction(ui->acSummariesBrowse);

    QMenu *menuSettings = menuBar()->addMenu(tr("Настройки"));
    if (AAnalyserBuild::isPPEnabled())
    {
        menuSettings->addAction(ui->acEditPersonalPrograms);
        menuSettings->addAction(ui->acEditDailyPrograms);
        menuSettings->addSeparator();
    }
    menuSettings->addAction(ui->acDeviceControl);
    menuSettings->addAction(ui->acSettings);
    menuSettings->addAction(ui->acClearGeometry);

    QMenu *menuColorShemes = menuSettings->addMenu(tr("Цветовые схемы"));
    m_agColorShemes = new QActionGroup(this);
    foreach (auto colSheme, ColorShemesList)
    {
        auto *ac = new QAction(ColorShemesName.value(colSheme));
        ac->setData(ColorShemesFile.value(colSheme));
        menuColorShemes->addAction(ac);
        m_agColorShemes->addAction(ac);
        ac->setCheckable(true);
        if (ColorShemesFile.value(colSheme) == m_curColorSheme->value().toString())
            ac->setChecked(true);
        connect(ac, &QAction::triggered, this, &MainWindow::on_selectColorSheme);
    }

    QMenu *menuHelp = menuBar()->addMenu(tr("Помощь"));
    menuHelp->addAction(ui->acAbout);

}

QMenu *MainWindow::initDatabaseClearMenu()
{
    QMenu *menu = new QMenu();

    return menu;
}

void MainWindow::initSelectDatabaseMenu()
{
    QString path = SettingsProvider::valueFromRegAppCopy("Database", "path").toString();
    QStringList folders = path.split("/");
    QString curNameBD = "";
    if (folders.size() > 2)
        curNameBD = folders.at(folders.size() - 2);

    m_menuSelectDatabase.setTitle(tr("Выбрать"));
    m_menuSelectDatabase.clear();
    auto dbInfos = DataDefines::getDatabases();
    foreach (auto dbInfo, dbInfos)
    {
        if (dbInfo.name != curNameBD)
        {
            QString title = dbInfo.comment;
            if (title == "")
                title = dbInfo.name;
            QAction *acSelDB = new QAction(title);
            acSelDB->setData(dbInfo.name);
            m_menuSelectDatabase.addAction(acSelDB);
            connect(acSelDB, &QAction::triggered, this, &MainWindow::on_selectDatabase);
        }
    }
}

void MainWindow::createClientWidgets()
{
    m_clientWidgets.clear();
    m_clientWidgets << new DataBaseResultWidget(this)
                    << new PersonalProgramWidget(this)
                    << new ExecuteWidget(this)
                    << new SignalAnalysisWidget(this)
                    << new SummariesWidget(this);

    foreach (auto* wgt, m_clientWidgets)
        ui->wgtClient->layout()->addWidget(wgt);

    auto cwv = SettingsProvider::valueFromRegAppCopy("MainWindow", "MainClientWidget", ClientWidgets::uidDatabaseResultWidgetUid).toString();
    if (!AAnalyserBuild::isPPEnabled() && cwv == ClientWidgets::uidPersonalProgramWidgetUid)
        cwv = ClientWidgets::uidDatabaseResultWidgetUid;
    showClientPage(cwv);
}


