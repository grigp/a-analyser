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

        auto valOpenEyes = calculator->factors(0)->factorValue(VectorFactorsDefines::KFRUid);
        ui->lblOpenEyesResult->setText(QString("%1 %").arg(valOpenEyes, 3, 'f', format));

        auto valCloseEyes = calculator->factors(1)->factorValue(VectorFactorsDefines::KFRUid);
        ui->lblCloseEyesResult->setText(QString("%1 %").arg(valCloseEyes, 3, 'f', format));

        auto valTarget = calculator->factors(2)->factorValue(VectorFactorsDefines::KFRUid);
        ui->lblTargetResult->setText(QString("%1 %").arg(valTarget, 3, 'f', format));

        getPersonalNorms();

        QList<DataDefines::GroupNormInfo> gni;
        if (static_cast<AAnalyserApplication*>(QApplication::instance())->getGroupNorms(ti.metodUid, ti.condition, gni))
        {
            foreach (auto gNorm, gni)
                qDebug() << gNorm.probeNum << gNorm.border << gNorm.conditionBorder;
        }

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


