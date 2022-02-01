#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "databaseresultwidget.h"
#include "executewidget.h"
#include "clientwidget.h"
#include "devicecontroldialog.h"
#include "datadefines.h"
#include "settingsprovider.h"
#include "databasepropertydialog.h"
#include "dataprovider.h"
#include "aanalysersettings.h"
#include "aanalyserdefines.h"
#include "aboutdialog.h"

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

    connect(ui->acSettings, &QAction::triggered, this, &MainWindow::onSettings);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showClientPage(const QString &uidPage)
{
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
            ui->menuBar->setVisible(static_cast<ClientWidget*>(wgt)->isExternalControl());
        }
    }
}

QWidget *MainWindow::getExecuteWidget()
{
    foreach (auto *wgt, m_clientWidgets)
        if (static_cast<ClientWidget*>(wgt)->uid() == ClientWidgets::uidExecuteWidgetUid)
            return wgt;
    return nullptr;
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
            showClientPage(ClientWidgets::uidDatabaseResultWidgetUid);
        event->ignore();
    }
    else
    {
        SettingsProvider::setValueToRegAppCopy("MainWindow", "Geometry", saveGeometry());
        event->accept();
    }
}

void MainWindow::onDeviceControl()
{
    DeviceControlDialog dlg(this);
    dlg.exec();
}

void MainWindow::onSettings()
{
    AAnalyserSettings dlg(this);
    dlg.exec();
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

    QMenu *menuSettings = menuBar()->addMenu(tr("Настройки"));
    menuSettings->addAction(ui->acDeviceControl);
    menuSettings->addAction(ui->acSettings);

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
                    << new ExecuteWidget(this);

    foreach (auto* wgt, m_clientWidgets)
        ui->wgtClient->layout()->addWidget(wgt);

    showClientPage(ClientWidgets::uidDatabaseResultWidgetUid);
}


