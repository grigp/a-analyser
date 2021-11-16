#include "stressstrategywidget.h"
#include "ui_stressstrategywidget.h"

#include "stressstrategycalculator.h"
#include "vectorfactors.h"
#include "dynamicdiagram.h"

StressStrategyWidget::StressStrategyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StressStrategyWidget)
{
    ui->setupUi(this);
}

StressStrategyWidget::~StressStrategyWidget()
{
    delete ui;
}

void StressStrategyWidget::calculate(StressStrategyCalculator *calculator, const QString &testUid)
{
    Q_UNUSED(testUid);
    auto valTarget = calculator->factors(0)->factorValue(VectorFactorsDefines::KFRUid);
    auto valZones = calculator->factors(1)->factorValue(VectorFactorsDefines::KFRUid);
    ui->lblTarget->setText(QString(tr("Мишень") + " %1 " + tr("%")).arg(QString::number(valTarget, 'f', 0)));
    ui->lblFiveZones->setText(QString(tr("С выделенной зоной") + " %1 " + tr("%")).arg(QString::number(valZones, 'f', 0)));

    QString resume = "";
    if (valTarget > valZones)
        resume = tr("Качество выполнения пробы мишень выше, чем качество выполнения пробы с выделенной зоной");
    else
    if (valTarget < valZones)
        resume = tr("Качество выполнения пробы с выделенной зоной выше, чем качество выполнения пробы мишень");
    else
    if (valTarget == valZones)
        resume = tr("Качество выполнения обеих проб одинаково");
    ui->lblStressStrategyResume->setText(resume);

    ui->wgtDiag->clear();
    ui->wgtDiag->setVolume(DynamicDiagram::Volume3D);
    ui->wgtDiag->setKind(DynamicDiagram::KindBar);
    auto itemTarget = new DiagItem(valTarget, tr("Мишень"));
    ui->wgtDiag->appendItem(itemTarget);
    auto itemZones = new DiagItem(valZones, tr("С выделенной зоной"));
    ui->wgtDiag->appendItem(itemZones);
}

void StressStrategyWidget::print(QPrinter *printer, const QString &testUid)
{

}

