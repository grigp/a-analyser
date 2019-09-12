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

QString DataBasePropertyDialog::comment() const
{
    return ui->edComment->text();
}

void DataBasePropertyDialog::setComment(const QString &comment)
{
    ui->edComment->setText(comment);
}
