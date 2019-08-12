#include "dopuskwidget.h"
#include "ui_dopuskwidget.h"

#include "dopuskcalculator.h"
#include "vectorfactors.h"
#include "aanalyserapplication.h"

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
    Q_UNUSED(testUid);
    auto format = static_cast<AAnalyserApplication*>(QApplication::instance())->
            getFactorInfo(VectorFactorsDefines::KFRUid).format();

    auto valOpenEyes = calculator->factors(0)->factorValue(VectorFactorsDefines::KFRUid);
    ui->lblOpenEyesResult->setText(QString("%1 %").arg(valOpenEyes, 3, 'f', format));

    auto valCloseEyes = calculator->factors(1)->factorValue(VectorFactorsDefines::KFRUid);
    ui->lblCloseEyesResult->setText(QString("%1 %").arg(valCloseEyes, 3, 'f', format));

    auto valTarget = calculator->factors(2)->factorValue(VectorFactorsDefines::KFRUid);
    ui->lblTargetResult->setText(QString("%1 %").arg(valTarget, 3, 'f', format));
}
