#include "databasepropertydialog.h"
#include "ui_databasepropertydialog.h"

DataBasePropertyDialog::DataBasePropertyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataBasePropertyDialog)
{
    ui->setupUi(this);
}

DataBasePropertyDialog::~DataBasePropertyDialog()
{
    delete ui;
}

void DataBasePropertyDialog::setPath(const QString &path)
{
    ui->lblPath->setText(path);
}

void DataBasePropertyDialog::setVersion(const QString &version)
{
    ui->lblVersion->setText(version);
}

QString DataBasePropertyDialog::comment() const
{
    return ui->edComment->text();
}

void DataBasePropertyDialog::setComment(const QString &comment)
{
    ui->edComment->setText(comment);
}
