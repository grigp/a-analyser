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

#include <QFile>
#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QAction>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/images/MainIcon2.ico"));
    createClientWidgets();

    initUi();
    initMenu();

    auto val = SettingsProvider::valueFromRegAppCopy("MainWindow", "Geometry").toByteArray();
    restoreGeometry(val);
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
    qDebug() << "import";

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

void MainWindow::initUi()
{
    QFile style( ":/qss/main.qss" );
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


