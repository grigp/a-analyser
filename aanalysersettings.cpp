#include "aanalysersettings.h"
#include "ui_aanalysersettings.h"

#include "aanalyserapplication.h"
#include "settingsprovider.h"

#include <QApplication>
#include <QDesktopWidget>
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
    fillListMonitors();
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
    auto winPatientNumber = SettingsProvider::valueFromRegAppCopy("", "PatientWindowNumber", static_cast<QVariant>(1)).toInt();
    ui->cbPatientWindowNumber->setCurrentText(QString::number(winPatientNumber));

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
    SettingsProvider::setValueToRegAppCopy("", "PatientWindowNumber", ui->cbPatientWindowNumber->currentText().toInt());

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

void AAnalyserSettings::fillListMonitors()
{
    auto app = static_cast<AAnalyserApplication*>(QApplication::instance());
    for (int i = 0; i < app->desktop()->screenCount(); ++i)
    {
        if (!app->desktop()->availableGeometry(i).contains(app->mainWindow()->geometry().center()))
            ui->cbPatientWindowNumber->addItem(QString::number(i + 1), i);
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
