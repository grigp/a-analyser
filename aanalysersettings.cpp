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
    ui->twTabs->setCurrentIndex(0);
    ui->cbUsePatientWindow->setVisible(false);
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

    auto winPatientNumber = SettingsProvider::valueFromRegAppCopy("", "PatientWindowNumber", static_cast<QVariant>(1)).toInt();
    ui->edPatientWindowNumber->setValue(winPatientNumber);

    auto country = SettingsProvider::valueFromRegAppCopy("UserLocalize", "Country", static_cast<QVariant>("")).toString();
    auto sity = SettingsProvider::valueFromRegAppCopy("UserLocalize", "Sity", static_cast<QVariant>("")).toString();
    fillSities(country, sity);
    if (m_idxCountry.isValid())
        ui->tvUserLocalize->expand(m_idxCountry);
    if (m_idxSity.isValid())
    {
        ui->tvUserLocalize->selectionModel()->select(m_idxSity, QItemSelectionModel::Select);
        ui->tvUserLocalize->scrollTo(m_idxSity);
    }
}

void AAnalyserSettings::save()
{
    SettingsProvider::setValueToRegAppCopy("", "PatientWindow", ui->cbUsePatientWindow->isChecked());

    SettingsProvider::setValueToRegAppCopy("", "PatientWindowNumber", ui->edPatientWindowNumber->value());

    auto selIdxs = ui->tvUserLocalize->selectionModel()->selectedIndexes();
    if ((selIdxs.size() > 0) && selIdxs.at(0).parent().isValid())
    {
        auto sity = selIdxs.at(0).data().toString();
        auto country = selIdxs.at(0).parent().data().toString();
        auto g = selIdxs.at(0).data(SMRAccelerationGravityRole).toDouble();

        SettingsProvider::setValueToRegAppCopy("UserLocalize", "Country", country);
        SettingsProvider::setValueToRegAppCopy("UserLocalize", "Sity", sity);
        SettingsProvider::setValueToRegAppCopy("UserLocalize", "g", g);
    }
}

void AAnalyserSettings::fillSities(const QString& country, const QString& sity)
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
            if (name == country)
                m_idxCountry = m_mdlSities.indexFromItem(itemC);

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
                if (name == sity)
                    m_idxSity = m_mdlSities.indexFromItem(itemS);
            }
        }
    }

    ui->tvUserLocalize->setModel(&m_mdlSities);
}
