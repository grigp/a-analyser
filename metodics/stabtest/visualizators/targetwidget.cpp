#include "targetwidget.h"
#include "ui_targetwidget.h"

#include "resultinfo.h"
#include "channelsdefines.h"
#include "targetcalculator.h"
#include "targetfactors.h"
#include "areaskg.h"
#include "dynamicdiagram.h"
#include "testresultdata.h"

#include <QTimer>
#include <QDebug>

TargetWidget::TargetWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TargetWidget)
{
    ui->setupUi(this);
}

TargetWidget::~TargetWidget()
{
    delete ui;
}

void TargetWidget::calculate(TargetCalculator *calculator, const QString &testUid)
{
    auto val = calculator->factors()->factorValue(TargetFactorsDefines::TargetScoreUid);
    auto valT = calculator->factors()->factorValue(TargetFactorsDefines::TargetTotalScoreUid);
    ui->lblScore->setText(QString(tr("Количество очков") + " %1").arg(QString::number(val, 'f', 0)));
    ui->lblTotalScore->setText(QString(tr("Суммарный балл") + " %1").arg(QString::number(valT, 'f', 0)));

    showSKG(calculator, testUid);
    showDiagram(calculator, testUid);
}

void TargetWidget::showSKG(TargetCalculator *calculator, const QString &testUid) const
{
    Q_UNUSED(calculator);
    auto* trd = new TestResultData();
    trd->openTest(testUid);
    Q_ASSERT(trd->probesCount() == 1);

    auto* probe = trd->probe(0);

    auto *sig = probe->signal(ChannelsDefines::chanStab);
    ui->wgtSKG->setSignal(sig);
    auto max = sig->absMaxValue();
    ui->wgtSKG->setVisibleMarker(false);

    QTimer::singleShot(30, [=]()
    {
        int diap = 1;
        int step = 0;
        while (diap < max)
        {
            diap = diap * 2;
            ++step;
        }
        ui->wgtSKG->setDiap(diap);
    });

//    auto angle = calculator->factors()->ellipse().angle;
//    auto sizeA = calculator->classicFactors(i)->ellipse().sizeA;
//    auto sizeB = calculator->classicFactors(i)->ellipse().sizeB;
    //    ui->wgtSKG->setEllipse(sizeA, sizeB, angle);
}

void TargetWidget::showDiagram(TargetCalculator *calculator, const QString &testUid) const
{
    Q_UNUSED(testUid);
    ui->wgtDiagram->setKind(DynamicDiagram::KindBar);
    ui->wgtDiagram->setVolume(DynamicDiagram::Volume3D);
    ui->wgtDiagram->setTitle(tr("Процент времени пребывания в зонах"));
    for (int i = 9; i >= 0; --i)
    {
        auto item = new DiagItem(calculator->factors()->histogram(i), QString::number(i + 1));
        ui->wgtDiagram->appendItem(item);
    }
}
