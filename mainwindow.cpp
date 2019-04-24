#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "databaseresultwidget.h"
#include "executewidget.h"
#include "clientwidget.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/images/MainIcon1.png"));
    createClientWidgets();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showClientPage(const QString &uidPage)
{
    foreach (auto *wgt, m_clientWidgets)
        wgt->setVisible(static_cast<ClientWidget*>(wgt)->uid() == uidPage);
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

void MainWindow::createClientWidgets()
{
    m_clientWidgets.clear();
    m_clientWidgets << new DataBaseResultWidget(this)
                    << new ExecuteWidget(this);

    foreach (auto* wgt, m_clientWidgets)
        ui->wgtClient->layout()->addWidget(wgt);

    showClientPage(ClientWidgets::uidDatabaseResultWidgetUid);
}
