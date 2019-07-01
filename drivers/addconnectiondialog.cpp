#include "addconnectiondialog.h"
#include "ui_addconnectiondialog.h"

#include "aanalyserapplication.h"

#include <QApplication>

AddConnectionDialog::AddConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddConnectionDialog)
{
    ui->setupUi(this);
    fill();
}

AddConnectionDialog::~AddConnectionDialog()
{
    delete ui;
}

QString AddConnectionDialog::driverUid() const
{
    return ui->cbDriver->currentData().toString();
}

QString AddConnectionDialog::driverName() const
{
    return ui->cbDriver->currentText();
}

DeviceProtocols::Ports AddConnectionDialog::port() const
{
    return static_cast<DeviceProtocols::Ports>(ui->cbPort->currentData().toInt());
}

QString AddConnectionDialog::comment() const
{
    return ui->edComment->text();
}

void AddConnectionDialog::selectDriver(int lineIdx)
{
    Q_UNUSED(lineIdx);
    ui->cbPort->clear();
    auto ports = m_ports.value(driverUid());

    for(int i = 0; i < ports.size(); ++i)
    {
        ui->cbPort->addItem(DeviceProtocols::portName(ports.at(i)), ports.at(i));
    }
}

void AddConnectionDialog::fill()
{
    auto app = static_cast<AAnalyserApplication*>(QApplication::instance());
    auto drivers = app->getDrivers();
    foreach (auto drvUid, drivers)
    {
        auto drvName = app->getDriverName(drvUid);
        ui->cbDriver->addItem(drvName, drvUid);

        auto ports = app->getDriverPorts(drvUid);
        m_ports.insert(drvUid, ports);
    }

    if (drivers.size() > 0)
    {
        ui->cbDriver->setCurrentIndex(0);
        selectDriver(0);
    }
}
