#include "aanalysersettings.h"
#include "ui_aanalysersettings.h"

#include "settingsprovider.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
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

    fillSities();
}

void AAnalyserSettings::save()
{
    SettingsProvider::setValueToRegAppCopy("", "PatientWindow", ui->cbUsePatientWindow->isChecked());
}

void AAnalyserSettings::fillSities()
{
    m_mdlSities.clear();

    QFile fileSities("sities.json");
    if (fileSities.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray ba = fileSities.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(ba));
        auto sobj = loadDoc.object();
        fileSities.close();

        auto arrCountrys = sobj["countrys"].toArray();
        for (int i = 0; i < arrCountrys.size(); ++i)
        {
            auto objCountry = arrCountrys.at(i).toObject();
            auto name = objCountry["name"].toString();

            auto itemC = new QStandardItem(name);
            itemC->setEditable(false);
            m_mdlSities.appendRow(itemC);

            auto arrSities = objCountry["sities"].toArray();
            for (int j = 0; j < arrSities.size(); ++j)
            {
                auto objSity = arrSities.at(j).toObject();
                auto name = objSity["name"].toString();
                auto g = objSity["g"].toDouble();

                auto itemS = new QStandardItem(name);
                itemS->setEditable(false);
                itemS->setData(g, SMRAccelerationGravityRole);
                itemC->appendRow(itemS);
            }
        }
    }


    ui->tvUserLocalize->setModel(&m_mdlSities);
}
