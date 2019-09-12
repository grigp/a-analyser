#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "databaseresultwidget.h"
#include "executewidget.h"
#include "clientwidget.h"
#include "devicecontroldialog.h"
#include "datadefines.h"
#include "settingsprovider.h"
#include "databasepropertydialog.h"

#include <QFile>
#include <QCloseEvent>
#include <QMessageBox>
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

void MainWindow::obDBDisconnected()
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
    dlg.setPath(SettingsProvider::valueFromRegAppCopy("Database", "path").toString());
    dlg.setComment(SettingsProvider::valueFromRegAppCopy("Database", "comment").toString());

    if (dlg.exec() == QDialog::Accepted)
    {
        SettingsProvider::setValueToRegAppCopy("Database", "comment", dlg.comment());
    }
}

void MainWindow::onDataBaseExport()
{
    qDebug() << "export";

}

void MainWindow::onDataBaseImport()
{
    qDebug() << "import";

}

void MainWindow::onDataBaseSelect()
{
    qDebug() << "select";

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
    menuDatabase->addSeparator();
    menuDatabase->addAction(ui->acDataBaseExport);
    menuDatabase->addAction(ui->acDataBaseImport);
    menuDatabase->addSeparator();
    menuDatabase->addAction(ui->acDataBaseSelect);

    QMenu *menuSettings = menuBar()->addMenu(tr("Настройки"));
    menuSettings->addAction(ui->acDeviceControl);
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


