#include "authorizationdialog.h"
#include "ui_authorizationdialog.h"

AuthorizationDialog::AuthorizationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthorizationDialog)
{
    ui->setupUi(this);
}

AuthorizationDialog::~AuthorizationDialog()
{
    delete ui;
}

void AuthorizationDialog::setTitle(const QString &title)
{
    ui->lblDialogTitle->setText(title);
}

void AuthorizationDialog::setInfo(const QString &info)
{
    ui->lblInfo->setText(info);
}

void AuthorizationDialog::setTextName(const QString &text)
{
    ui->lblName->setText(text);
}

void AuthorizationDialog::setTextPassword(const QString &text)
{
    ui->lblPassword->setText(text);
}

void AuthorizationDialog::setLogotip(QPixmap &pixmap)
{
    ui->lblLogotip->setPixmap(pixmap);
}
