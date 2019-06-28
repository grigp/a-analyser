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

QString AddConnectionDialog::port() const
{
    return ui->cbPort->currentText();
}

QString AddConnectionDialog::comment() const
{
    return ui->edComment->text();
}

void AddConnectionDialog::fill()
{
    auto app = static_cast<AAnalyserApplication*>(QApplication::instance());
    auto drivers = app->getDrivers();
    foreach (auto drvUid, drivers)
    {
        auto drvName = app->getDriverName(drvUid);
        ui->cbDriver->addItem(drvName, drvUid);
    }
}
