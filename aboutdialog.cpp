#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include "userinfodialog.h"
#include "settingsprovider.h"

#include <QBuffer>
#include <QDebug>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    ui->lblUrl->setText("<a href=\"http://a-med.tech/\">" + tr("Посетить сайт разработчика") + "</a>");
    ui->lblAppName->setStyleSheet("font-size: 24pt; color: rgb(0, 0, 255);");
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_UserinfoDialogClicked()
{
    UserInfoDialog dlg(this);

    auto fio = new SettingsValue("UserData", "FIO", "");
    dlg.setFio(fio->value().toString());
    auto enterprice = new SettingsValue("UserData", "Enterprice", "");
    dlg.setEnterprice(enterprice->value().toString());
    auto address = new SettingsValue("UserData", "Address", "");
    dlg.setAddress(address->value().toString());
    auto sity = new SettingsValue("UserData", "Sity", "");
    dlg.setSity(sity->value().toString());
    auto phones = new SettingsValue("UserData", "Phones", "");
    dlg.setPhones(phones->value().toString());
    auto email = new SettingsValue("UserData", "Email", "");
    dlg.setEmail(email->value().toString());
    auto website = new SettingsValue("UserData", "Website", "");
    dlg.setWebsite(website->value().toString());

    auto logotip = new SettingsValue("UserData", "Logotip");
    QPixmap p;
    p.loadFromData(logotip->value().toByteArray(), "PNG");
    dlg.setLogotip(p);

    connect(&dlg, &UserInfoDialog::accepted, [&]()
    {
        fio->set(dlg.fio());
        enterprice->set(dlg.enterprice());
        address->set(dlg.address());
        sity->set(dlg.sity());
        phones->set(dlg.phones());
        email->set(dlg.email());
        website->set(dlg.website());

        auto pix = dlg.logotip();
        QByteArray ba;
        QBuffer buffer(&ba);
        buffer.open(QIODevice::WriteOnly);
        pix.save(&buffer, "PNG");
        logotip->set(ba);
    });
    dlg.exec();
}
