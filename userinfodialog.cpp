#include "userinfodialog.h"
#include "ui_userinfodialog.h"

#include <QFileDialog>
#include <QDebug>

UserInfoDialog::UserInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserInfoDialog)
{
    ui->setupUi(this);
}

UserInfoDialog::~UserInfoDialog()
{
    delete ui;
}

void UserInfoDialog::onLogoBrowseClicked()
{
    auto fn = QFileDialog::getOpenFileName(this, tr("Выберите картинку логотипа"), "", tr("Файлы картинок (*.png)"));
    if (fn != "")
        ui->lblLogoPreview->setPixmap(QPixmap(fn));
}

QString UserInfoDialog::fio() const
{
    return ui->edFIO->text();
}

void UserInfoDialog::setFio(const QString &fio)
{
    ui->edFIO->setText(fio);
}

QString UserInfoDialog::enterprice() const
{
    return ui->edEnterprice->text();
}

void UserInfoDialog::setEnterprice(const QString &enterprice)
{
    ui->edEnterprice->setText(enterprice);
}

QString UserInfoDialog::address() const
{
    return ui->edAddress->text();
}

void UserInfoDialog::setAddress(const QString &address)
{
    ui->edAddress->setText(address);
}

QString UserInfoDialog::sity() const
{
    return ui->edSity->text();
}

void UserInfoDialog::setSity(const QString &sity)
{
    ui->edSity->setText(sity);
}

QString UserInfoDialog::phones() const
{
    return ui->edPhones->text();
}

void UserInfoDialog::setPhones(const QString &phones)
{
    ui->edPhones->setText(phones);
}

QString UserInfoDialog::email() const
{
    return ui->edEmail->text();
}

void UserInfoDialog::setEmail(const QString &email)
{
    ui->edEmail->setText(email);
}

QString UserInfoDialog::website() const
{
    return ui->edWebsite->text();
}

void UserInfoDialog::setWebsite(const QString &website)
{
    ui->edWebsite->setText(website);
}

QPixmap UserInfoDialog::logotip()
{
    return ui->lblLogoPreview->pixmap()->copy();
}

void UserInfoDialog::setLogotip(QPixmap logotip)
{
    ui->lblLogoPreview->setPixmap(logotip);
}
