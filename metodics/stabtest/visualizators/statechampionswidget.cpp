#include "statechampionswidget.h"
#include "ui_statechampionswidget.h"

#include "statechampionstestcalculator.h"
#include "aanalyserapplication.h"
#include "vectorfactors.h"
#include "classicfactors.h"
#include "ratioprobesfactors.h"

#include <QDebug>

StateChampionsWidget::StateChampionsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StateChampionsWidget)
{
    ui->setupUi(this);
}

StateChampionsWidget::~StateChampionsWidget()
{
    delete ui;
}

void StateChampionsWidget::calculate(StateChampionsTestCalculator *calculator,
                                     const QString &testUid)
{
    Q_UNUSED(testUid);

    auto fmtS = static_cast<AAnalyserApplication*>(QApplication::instance())->
            getFactorInfo(ClassicFactorsDefines::SquareUid).format();
    auto fmtV = static_cast<AAnalyserApplication*>(QApplication::instance())->
            getFactorInfo(VectorFactorsDefines::VMidUid).format();
    auto fmtKFR = static_cast<AAnalyserApplication*>(QApplication::instance())->
            getFactorInfo(VectorFactorsDefines::KFRUid).format();
    auto fmtRK = static_cast<AAnalyserApplication*>(QApplication::instance())->
            getFactorInfo(RatioProbesFactorsDefines::Probe2SUid).format();

    ui->lblSquareProbe1->setText(QString::number(calculator->square(0), 'f', fmtS));
    ui->lblSquareProbe2->setText(QString::number(calculator->square(1), 'f', fmtS));
    ui->lblSquareProbe3->setText(QString::number(calculator->square(2), 'f', fmtS));
    ui->lblSpeedProbe1->setText(QString::number(calculator->speed(0), 'f', fmtV));
    ui->lblSpeedProbe2->setText(QString::number(calculator->speed(1), 'f', fmtV));
    ui->lblSpeedProbe3->setText(QString::number(calculator->speed(2), 'f', fmtV));
    ui->lblKFRProbe3->setText(QString::number(calculator->targetKFR(), 'f', fmtKFR));
    ui->lblRombKoefValue->setText(QString::number(calculator->rombKoef(), 'f', fmtRK));
}
