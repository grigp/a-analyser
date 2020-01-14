#include "aanalysersettings.h"
#include "ui_aanalysersettings.h"

#include "settingsprovider.h"

#include <QDebug>

AAnalyserSettings::AAnalyserSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AAnalyserSettings)
{
    ui->setupUi(this);
}

AAnalyserSettings::~AAnalyserSettings()
{
    delete ui;
}

int AAnalyserSettings::exec()
{
    load();
    return QDialog::exec();
}

void AAnalyserSettings::accept()
{
    save();
    QDialog::accept();
}


void AAnalyserSettings::load()
{
    auto winPresent = SettingsProvider::valueFromRegAppCopy("", "PatientWindow", static_cast<QVariant>(true)).toBool();
    ui->cbUsePatientWindow->setChecked(winPresent);
}

void AAnalyserSettings::save()
{
    SettingsProvider::setValueToRegAppCopy("", "PatientWindow", ui->cbUsePatientWindow->isChecked());
}
