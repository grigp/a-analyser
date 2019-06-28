#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "databaseresultwidget.h"
#include "executewidget.h"
#include "clientwidget.h"
#include "devicecontroldialog.h"

#include <QFile>
#include <QCloseEvent>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/images/MainIcon1.png"));
    createClientWidgets();

    initUi();
    initMenu();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showClientPage(const QString &uidPage)
{
    foreach (auto *wgt, m_clientWidgets)
    {
        wgt->setVisible(static_cast<ClientWidget*>(wgt)->uid() == uidPage);
        if (static_cast<ClientWidget*>(wgt)->uid() == uidPage)
            m_currentClientPage = uidPage;
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
        event->accept();
}

void MainWindow::onDeviceControl()
{
    DeviceControlDialog dlg(this);
    dlg.exec();
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
