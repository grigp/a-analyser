#include "aanalysersettings.h"
#include "ui_aanalysersettings.h"

#include "aanalyserapplication.h"
#include "settingsprovider.h"
#include "basedefines.h"

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
    fillRunningMode();
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

void AAnalyserSettings::cbOnePatientClicked(bool checked)
{
    ui->edOnePatientFIO->setEnabled(checked);
    ui->lblOnePatientFIO->setEnabled(checked);
}

void AAnalyserSettings::on_runningModeChange(int idx)
{
    ui->frTimeCounter->setEnabled(static_cast<BaseDefines::RunningMode>(idx) == BaseDefines::rmAutomatic);
    ui->frTimeLatent->setEnabled(static_cast<BaseDefines::RunningMode>(idx) == BaseDefines::rmAutomatic);
}

void AAnalyserSettings::load()
{
    m_winPatientNumber = SettingsProvider::valueFromRegAppCopy("", AAnalyserSettingsParams::pn_patientWindowNumber, static_cast<QVariant>(1)).toInt();
    ui->cbPatientWindowNumber->setCurrentText(QString::number(m_winPatientNumber));

    m_isOnePatient = SettingsProvider::valueFromRegAppCopy("", AAnalyserSettingsParams::pn_onePatientMode, static_cast<QVariant>(false)).toBool();
    ui->cbOnePatient->setChecked(m_isOnePatient);
    cbOnePatientClicked(m_isOnePatient);
    m_onePatientFIO = SettingsProvider::valueFromRegAppCopy("", AAnalyserSettingsParams::pn_onePatientFIO).toString();
    ui->edOnePatientFIO->setText(m_onePatientFIO);

    m_country = SettingsProvider::valueFromRegAppCopy(AAnalyserSettingsParams::pc_userLocalize,
                                                      AAnalyserSettingsParams::pn_country,
                                                      static_cast<QVariant>("")).toString();
    m_sity = SettingsProvider::valueFromRegAppCopy(AAnalyserSettingsParams::pc_userLocalize,
                                                   AAnalyserSettingsParams::pn_sity,
                                                   static_cast<QVariant>("")).toString();
    fillSities(m_country, m_sity);
    if (m_idxCountry.isValid())
        ui->tvUserLocalize->expand(m_idxCountry);
    if (m_idxSity.isValid())
    {
        ui->tvUserLocalize->selectionModel()->select(m_idxSity, QItemSelectionModel::Select);
        ui->tvUserLocalize->scrollTo(m_idxSity);
    }

    auto rm = SettingsProvider::valueFromRegAppCopy("", AAnalyserSettingsParams::pn_runningMode, BaseDefines::rmOperator).toInt();
    ui->cbRunningMode->setCurrentIndex(rm);
    on_runningModeChange(rm);

    auto tm = SettingsProvider::valueFromRegAppCopy("", AAnalyserSettingsParams::pn_timeCounter, 5).toInt();
    ui->edTimeCounter->setValue(tm);

    auto l = SettingsProvider::valueFromRegAppCopy("", AAnalyserSettingsParams::pn_timeLatent, 2).toInt();
    ui->edTimeLatent->setValue(l);
}

void AAnalyserSettings::save()
{
    auto pwn = ui->cbPatientWindowNumber->currentText().toInt();
    SettingsProvider::setValueToRegAppCopy("",  AAnalyserSettingsParams::pn_patientWindowNumber, pwn);
    if (pwn != m_winPatientNumber)
        static_cast<AAnalyserApplication*>(QApplication::instance())->changeApplicationParam("", AAnalyserSettingsParams::pn_patientWindowNumber, pwn);

    auto opm = ui->cbOnePatient->isChecked();
    SettingsProvider::setValueToRegAppCopy("", AAnalyserSettingsParams::pn_onePatientMode, opm);
    if (opm != m_isOnePatient)
        static_cast<AAnalyserApplication*>(QApplication::instance())->changeApplicationParam("", AAnalyserSettingsParams::pn_onePatientMode, opm);

    auto opfio = ui->edOnePatientFIO->text();
    SettingsProvider::setValueToRegAppCopy("", AAnalyserSettingsParams::pn_onePatientFIO, opfio);
    if (opfio != m_onePatientFIO)
        static_cast<AAnalyserApplication*>(QApplication::instance())->changeApplicationParam("", AAnalyserSettingsParams::pn_onePatientFIO, opfio);


    auto selIdxs = ui->tvUserLocalize->selectionModel()->selectedIndexes();
    if ((selIdxs.size() > 0) && selIdxs.at(0).parent().isValid())
    {
        auto sity = selIdxs.at(0).data().toString();
        auto country = selIdxs.at(0).parent().data().toString();
        auto g = selIdxs.at(0).data(SMRAccelerationGravityRole).toDouble();

        SettingsProvider::setValueToRegAppCopy(AAnalyserSettingsParams::pc_userLocalize, AAnalyserSettingsParams::pn_country, country);
        SettingsProvider::setValueToRegAppCopy(AAnalyserSettingsParams::pc_userLocalize, AAnalyserSettingsParams::pn_sity, sity);
        SettingsProvider::setValueToRegAppCopy(AAnalyserSettingsParams::pc_userLocalize, AAnalyserSettingsParams::pn_g, g);
    }

    auto rm = ui->cbRunningMode->currentIndex();
    SettingsProvider::setValueToRegAppCopy("", AAnalyserSettingsParams::pn_runningMode, rm);

    auto tm = ui->edTimeCounter->value();
    SettingsProvider::setValueToRegAppCopy("", AAnalyserSettingsParams::pn_timeCounter, tm);

    auto l = ui->edTimeLatent->value();
    SettingsProvider::setValueToRegAppCopy("", AAnalyserSettingsParams::pn_timeLatent, l);
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

void AAnalyserSettings::fillRunningMode()
{
    ui->cbRunningMode->clear();
    ui->cbRunningMode->addItem(tr("с оператором"), BaseDefines::rmOperator);
    ui->cbRunningMode->addItem(tr("автоматический"), BaseDefines::rmAutomatic);
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
