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

        m_values[0] = calculator->factors(0)->factorValue(VectorFactorsDefines::KFRUid);
        ui->lblOpenEyesResult->setText(QString("%1 %").arg(m_values[0], 3, 'f', format));

        m_values[1] = calculator->factors(1)->factorValue(VectorFactorsDefines::KFRUid);
        ui->lblCloseEyesResult->setText(QString("%1 %").arg(m_values[1], 3, 'f', format));

        m_values[2] = calculator->factors(2)->factorValue(VectorFactorsDefines::KFRUid);
        ui->lblTargetResult->setText(QString("%1 %").arg(m_values[2], 3, 'f', format));

        getPersonalNorms();

        QList<DataDefines::GroupNormInfo> gni;
        if (static_cast<AAnalyserApplication*>(QApplication::instance())->getGroupNorms(ti.metodUid, ti.condition, gni))
        {
            m_groupNorms.clear();
            foreach (auto gNorm, gni)
                m_groupNorms.insert(gNorm.probeNum, GroupNorm(gNorm.border, gNorm.conditionBorder));
        }

        showConslution();

        connect(static_cast<AAnalyserApplication*>(QApplication::instance()),
                &AAnalyserApplication::recalculatedPersonalNorm,
                this, &DopuskWidget::on_recalculatedPersonalNorm);
    }
}

void DopuskWidget::on_recalculatedPersonalNorm(const QString &patientUid,
                                               const QString &methodUid,
                                               const QString &conditionUid)
{
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(m_testUid, ti))
    {
        if (patientUid == ti.patientUid && methodUid == ti.metodUid && conditionUid == ti.condition)
        {
            getPersonalNorms();
        }
    }
}

void DopuskWidget::getPersonalNorms()
{
    if (static_cast<AAnalyserApplication*>(QApplication::instance())->getPersonalNorm(m_testUid, m_pnil))
    {
        showConslution();

        for (int i = 0; i < m_pnil.size(); ++i)
        {
            QString st = QString(tr("Норма") + " %1 (q = %2)").arg(m_pnil.at(i).value).arg(m_pnil.at(i).stdDeviation);
            if (i == 0)
            {
                ui->lblOpenEyesNorm->setText(st);
            }
            else
            if (i == 1)
            {
                ui->lblCloseEyesNorm->setText(st);
            }
            else
            if (i == 2)
            {
                ui->lblTargetNorm->setText(st);
            }
        }
    }
}

void DopuskWidget::showConslution()
{
    for (int i = 0; i < 3; ++i)
    {
        GroupNorm gnv(m_groupNorms.value(i).bound, m_groupNorms.value(i).conditionBound);
        NormValue nv = NotNormal;
        if (m_values[i] >= gnv.bound)
            nv = Normal;
        else
        if (m_values[i] < gnv.bound && m_values[i] > gnv.conditionBound)
            nv = ConditionNormal;

        QColor color = Qt::gray;
        if (nv == Normal)
            color = Qt::green;
        else
        if (nv == 1)
            color = Qt::yellow;
        else
        if (nv == 2)
            color = Qt::red;

        if (i == 0)
            ui->wgtOpenEyesGroupNorm->setColor(color);
        else
        if (i == 1)
            ui->wgtCloseEyesGroupNorm->setColor(color);
        else
        if (i == 2)
            ui->wgtCloseEyesGroupNorm->setColor(color);
    }

}


