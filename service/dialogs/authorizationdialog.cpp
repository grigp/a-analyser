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

QString AuthorizationDialog::name() const
{
    return ui->edName->text();
}

void AuthorizationDialog::setName(const QString &name)
{
    ui->edName->setText(name);
}

QString AuthorizationDialog::password() const
{
    return ui->edPassword->text();
}

void AuthorizationDialog::setPassword(const QString &password)
{
    ui->edPassword->setText(password);
}

void AuthorizationDialog::on_showPassword(bool visible)
{
    if (visible)
        ui->edPassword->setEchoMode(QLineEdit::Normal);
    else
        ui->edPassword->setEchoMode(QLineEdit::Password);
}
