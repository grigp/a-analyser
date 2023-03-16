#include "targetwidget.h"
#include "ui_targetwidget.h"

#include "resultinfo.h"
#include "channelsdefines.h"
#include "targetcalculator.h"
#include "targetfactors.h"
#include "areaskg.h"
#include "dynamicdiagram.h"
#include "testresultdata.h"
#include "reportelements.h"

#include <QTimer>
#include <QDebug>

namespace
{
DynamicDiagram *wgtDiagram {nullptr};
AreaSKG *wgtSKG {nullptr};
QString sScore {""};
QString sTotalScore {""};
}

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
    sScore = QString(tr("Количество очков") + " %1").arg(QString::number(val, 'f', 0));
    sTotalScore = QString(tr("Суммарный балл") + " %1").arg(QString::number(valT, 'f', 0));
    ui->lblScore->setText(sScore);
    ui->lblTotalScore->setText(sTotalScore);

    showSKG(calculator, testUid);
    showDiagram(calculator, testUid);
}

void TargetWidget::print(QPrinter *printer, const QString &testUid)
{
    QPainter *painter = new QPainter(printer);
    QRect paper = printer->pageRect();

    painter->begin(printer);
    //! Заголовок
    QRect rectHeader(paper.x() + paper.width() / 20, paper.y() + paper.height() / 30, paper.width() / 20 * 18, paper.height() / 30 * 3);
    ReportElements::drawHeader(painter, testUid, rectHeader);

    if (printer->orientation() == QPrinter::Portrait)
    {
        painter->setFont(QFont("Sans", 16, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 10,
                          static_cast<int>(paper.y() + paper.height() / 10 * 1.6),
                          sScore);
        painter->setFont(QFont("Sans", 12, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 10,
                          static_cast<int>(paper.y() + paper.height() / 10 * 1.8),
                          sTotalScore);

        ReportElements::drawWidget(painter, wgtDiagram,
                                   static_cast<int>(paper.width() * 0.85), static_cast<int>(paper.height() * 0.85),
                                   paper.x() + paper.width() / 10, paper.y() + paper.height() / 10 * 2);
        ReportElements::drawWidget(painter, wgtSKG,
                                   static_cast<int>(paper.width() * 0.45), static_cast<int>(paper.height() * 0.45),
                                   paper.x() + paper.width() / 10 * 3, paper.y() + paper.height() / 10 * 5);
    }
    else
    if (printer->orientation() == QPrinter::Landscape)
    {
        painter->setFont(QFont("Sans", 16, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 10,
                          static_cast<int>(paper.y() + paper.height() / 10 * 2),
                          sScore);
        painter->setFont(QFont("Sans", 12, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 10,
                          static_cast<int>(paper.y() + paper.height() / 10 * 2.4),
                          sTotalScore);

        ReportElements::drawWidget(painter, wgtDiagram,
                                   static_cast<int>(paper.width() * 0.6), static_cast<int>(paper.height() * 0.6),
                                   paper.x() + paper.width() / 12, paper.y() + paper.height() / 10 * 3);
        ReportElements::drawWidget(painter, wgtSKG,
                                   static_cast<int>(paper.width() * 0.4), static_cast<int>(paper.height() * 0.4),
                                   static_cast<int>(paper.x() + paper.width() / 10 * 6.5), paper.y() + paper.height() / 10 * 3);
    }

    //! Нижний колонтитул
    QRect rectFooter(paper.x() + paper.width() / 20,
                     paper.y() + paper.height() - static_cast<int>(paper.height() / 30 * 1.5),
                     paper.width() / 20 * 18,
                     static_cast<int>(paper.height() / 30 * 1.5));
    ReportElements::drawFooter(painter, testUid, rectFooter);

    painter->end();
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
    ui->wgtSKG_->setSignal(sig);
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
        ui->wgtSKG_->setDiap(diap);
    });

    wgtSKG = ui->wgtSKG;

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
    wgtDiagram = ui->wgtDiagram;
}
