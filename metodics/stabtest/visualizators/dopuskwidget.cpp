#include "dopuskwidget.h"
#include "ui_dopuskwidget.h"

#include "dopuskcalculator.h"
#include "vectorfactors.h"
#include "aanalyserapplication.h"
#include "dataprovider.h"

#include <QDebug>

DopuskWidget::DopuskWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DopuskWidget)
{
    ui->setupUi(this);
}

DopuskWidget::~DopuskWidget()
{
    delete ui;
}

void DopuskWidget::calculate(DopuskCalculator *calculator, const QString &testUid)
{
    m_testUid = testUid;
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(m_testUid, ti))
    {
        auto format = static_cast<AAnalyserApplication*>(QApplication::instance())->
                getFactorInfo(VectorFactorsDefines::KFRUid).format();

        QString fctName = static_cast<AAnalyserApplication*>(QApplication::instance())->
                getFactorInfo(VectorFactorsDefines::KFRUid).name();

        m_values[0] = calculator->factors(0)->factorValue(VectorFactorsDefines::KFRUid);
        ui->lblOpenEyesResult->setText(QString("%1 %").arg(m_values[0], 3, 'f', format));
        ui->lblOpenEyesFactor->setText(fctName);

        m_values[1] = calculator->factors(1)->factorValue(VectorFactorsDefines::KFRUid);
        ui->lblCloseEyesResult->setText(QString("%1 %").arg(m_values[1], 3, 'f', format));
        ui->lblCloseEyesFactor->setText(fctName);

        m_values[2] = calculator->factors(2)->factorValue(VectorFactorsDefines::KFRUid);
        ui->lblTargetResult->setText(QString("%1 %").arg(m_values[2], 3, 'f', format));
        ui->lblTargetFactor->setText(fctName);

        getPersonalNorms();
        getGroupNorm(ti.metodUid, ti.condition);

        showConslution();

        connect(static_cast<AAnalyserApplication*>(QApplication::instance()),
                &AAnalyserApplication::personalNormRecalculated,
                this, &DopuskWidget::on_changePersonalNorm);
        connect(static_cast<AAnalyserApplication*>(QApplication::instance()),
                &AAnalyserApplication::personalNormDeleted,
                this, &DopuskWidget::on_changePersonalNorm);
    }
}

void DopuskWidget::on_changePersonalNorm(const QString &patientUid,
                                         const QString &methodUid,
                                         const QString &conditionUid)
{
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(m_testUid, ti))
    {
        if (patientUid == ti.patientUid && methodUid == ti.metodUid && conditionUid == ti.condition)
        {
            getGroupNorm(methodUid, conditionUid);
            getPersonalNorms();
        }
    }
}

void DopuskWidget::getGroupNorm(const QString &methodUid, const QString &conditionUid)
{
    m_groupNorms.clear();
    QList<DataDefines::GroupNormInfo> gni;
    if (static_cast<AAnalyserApplication*>(QApplication::instance())->getGroupNorms(methodUid, conditionUid, gni))
    {
        foreach (auto gNorm, gni)
            m_groupNorms.insert(gNorm.probeNum, GroupNorm(gNorm.border, gNorm.conditionBorder));
    }
}

void DopuskWidget::getPersonalNorms()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->getPersonalNorm(m_testUid, m_pnil);
    showConslution();
}

void DopuskWidget::showConslution()
{
    for (int i = 0; i < 3; ++i)
    {
        NormBounds pn(-1, -1, -1, -1);
        if (i < m_pnil.size())
        {
            pn.normValLo = m_pnil.at(i).value - m_pnil.at(i).stdDeviation * 1;
            pn.normValHi = m_pnil.at(i).value + m_pnil.at(i).stdDeviation * 1;
            pn.condNormLo = m_pnil.at(i).value - m_pnil.at(i).stdDeviation * 2;
            pn.condNormHi = m_pnil.at(i).value + m_pnil.at(i).stdDeviation * 2;
        }
        DataDefines::NormValue pnv = getPersonalNormValue(i, pn);
        QColor pnColor = DataDefines::normValueToColor(pnv);

        DataDefines::NormValue gnv = getGroupNormValue(i);
        QColor gnColor = DataDefines::normValueToColor(gnv);

        auto drawNorm = [&](RGBLed *gnLed, RGBLed *pnLed,
                QLabel *gnResume, QLabel *pnResume,
                CircleNormIndicator *cni)
        {
            gnLed->setColor(gnColor);
            gnResume->setText(DataDefines::normValueToString(gnv));
            QPalette palette = gnResume->palette();
            palette.setColor(gnResume->foregroundRole(), DataDefines::normValueToColorDark(gnv));
            gnResume->setPalette(palette);

            pnLed->setColor(pnColor);
            pnResume->setText(DataDefines::normValueToString(pnv));
            palette = pnResume->palette();
            palette.setColor(pnResume->foregroundRole(), DataDefines::normValueToColorDark(pnv));
            pnResume->setPalette(palette);

            cni->setValue(m_values[i]);
            if (m_groupNorms.contains(i))
            {
                NormBounds gn(m_groupNorms.value(i).conditionBound, m_groupNorms.value(i).bound, 100, 100);
                cni->setGroupNorm(gn);
            }
            else
            {
                NormBounds gn(-1, -1, -1, -1);
                cni->setGroupNorm(gn);
            }
            cni->setPersonalNorm(pn);
        };

        if (i == 0)
            drawNorm(ui->wgtOpenEyesGroupNorm, ui->wgtOpenEyesPersonalNorm,
                     ui->lblOpenEyesGroupNormResume, ui->lblOpenEyesPersonalNormResume,
                     ui->wgtOpenEyesNorm);
        else
        if (i == 1)
            drawNorm(ui->wgtCloseEyesGroupNorm, ui->wgtCloseEyesPersonalNorm,
                     ui->lblCloseEyesGroupNormResume, ui->lblCloseEyesPersonalNormResume,
                     ui->wgtCloseEyesNorm);
        else
        if (i == 2)
            drawNorm(ui->wgtTargetGroupNorm, ui->wgtTargetPersonalNorm,
                     ui->lblTargetGroupNormResume, ui->lblTargetPersonalNormResume,
                     ui->wgtTargetNorm);
    }

    //! Количество нормообразующих обследований
    auto cnt = static_cast<AAnalyserApplication*>(QApplication::instance())->
            getPersonalNormContainedTestCount(m_testUid);
    ui->lblNormContainsCount->setText(QString(tr("Количество нормообразующих обследований") + " - %1").arg(cnt));
}

DataDefines::NormValue DopuskWidget::getGroupNormValue(const int numProbe)
{
    DataDefines::NormValue gnv = DataDefines::MissingNorm;
    if (m_groupNorms.contains(numProbe))
    {
        GroupNorm gnvalues(m_groupNorms.value(numProbe).bound, m_groupNorms.value(numProbe).conditionBound);
        if (m_values[numProbe] >= gnvalues.bound)
            gnv = DataDefines::Normal;
        else
        if (m_values[numProbe] < gnvalues.bound && m_values[numProbe] > gnvalues.conditionBound)
            gnv = DataDefines::ConditionNormal;
        else
        if (m_values[numProbe] < gnvalues.conditionBound)
            gnv = DataDefines::NotNormal;
    }
    return gnv;
}

DataDefines::NormValue DopuskWidget::getPersonalNormValue(const int numProbe, const NormBounds &pn)
{
    if (pn.normValLo == -1 || pn.normValHi == -1 || pn.condNormLo == -1 || pn.condNormHi == -1)
        return DataDefines::MissingNorm;
    else
    if (m_values[numProbe] < pn.condNormLo || m_values[numProbe] > pn.condNormHi)
        return DataDefines::NotNormal;
    else
    if (m_values[numProbe] < pn.normValLo || m_values[numProbe] > pn.normValHi)
        return DataDefines::ConditionNormal;
    else
        return DataDefines::Normal;
}


