#include "stressstrategywidget.h"
#include "ui_stressstrategywidget.h"

#include "stressstrategycalculator.h"
#include "vectorfactors.h"
#include "dynamicdiagram.h"
#include "reportelements.h"

namespace
{
DynamicDiagram *wgtDiag {nullptr};

QString sTarget {""};
QString sFiveZones {""};
QString sStressStrategyResume {""};

}

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
    sTarget = QString(tr("Мишень") + " %1 " + tr("%")).arg(QString::number(valTarget, 'f', 0));
    sFiveZones = QString(tr("С выделенной зоной") + " %1 " + tr("%")).arg(QString::number(valZones, 'f', 0));
    ui->lblTarget->setText(sTarget);
    ui->lblFiveZones->setText(sFiveZones);

    sStressStrategyResume = "";
    if (valTarget > valZones)
        sStressStrategyResume = tr("Качество выполнения пробы мишень выше, чем качество выполнения пробы с выделенной зоной");
    else
    if (valTarget < valZones)
        sStressStrategyResume = tr("Качество выполнения пробы с выделенной зоной выше, чем качество выполнения пробы мишень");
    else
    if (fabs(valTarget - valZones) < 2)
        sStressStrategyResume = tr("Качество выполнения обеих проб одинаково");

    ui->lblStressStrategyResume->setText(sStressStrategyResume);

    ui->wgtDiag->clear();
    ui->wgtDiag->setVolume(DynamicDiagramDefines::Volume3D);
    ui->wgtDiag->setKind(DynamicDiagramDefines::KindBar);
    auto itemTarget = new DiagItem(valTarget, tr("Мишень"));
    ui->wgtDiag->appendItem(itemTarget);
    auto itemZones = new DiagItem(valZones, tr("С выделенной зоной"));
    ui->wgtDiag->appendItem(itemZones);

    wgtDiag = ui->wgtDiag;
}

void StressStrategyWidget::print(QPrinter *printer, const QString &testUid)
{
    QPainter *painter = new QPainter(printer);
    QRect paper = printer->pageRect();

    painter->begin(printer);
    //! Заголовок
    QRect rectHeader(paper.x() + paper.width() / 20, paper.y() + paper.height() / 30, paper.width() / 20 * 18, paper.height() / 30 * 3);
    ReportElements::drawHeader(painter, testUid, rectHeader);

    if (printer->orientation() == QPrinter::Portrait)
    {
        painter->setFont(QFont("Sans", 10, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 12,
                          static_cast<int>(paper.y() + paper.height() / 10 * 1.6),
                          tr("Качество выполнения задания"));
        painter->drawText(paper.x() + paper.width() / 12,
                          static_cast<int>(paper.y() + paper.height() / 10 * 1.9),
                          sTarget);
        painter->drawText(paper.x() + paper.width() / 12,
                          static_cast<int>(paper.y() + paper.height() / 10 * 2.2),
                          sFiveZones);
        painter->drawText(paper.x() + paper.width() / 12,
                          static_cast<int>(paper.y() + paper.height() / 10 * 2.5),
                          sStressStrategyResume);

        ReportElements::drawWidget(painter, wgtDiag,
                                   static_cast<int>(paper.width() * 0.85), static_cast<int>(paper.height() * 0.85),
                                   paper.x() + paper.width() / 12, paper.y() + paper.height() / 10 * 3);
    }
    else
    if (printer->orientation() == QPrinter::Landscape)
    {
        painter->setFont(QFont("Sans", 10, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 12,
                          static_cast<int>(paper.y() + paper.height() / 10 * 1.7),
                          tr("Качество выполнения задания"));
        painter->drawText(paper.x() + paper.width() / 12,
                          static_cast<int>(paper.y() + paper.height() / 10 * 2.0),
                          sTarget);
        painter->drawText(paper.x() + paper.width() / 12,
                          static_cast<int>(paper.y() + paper.height() / 10 * 2.3),
                          sFiveZones);
        painter->setFont(QFont("Sans", 14, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 12,
                          static_cast<int>(paper.y() + paper.height() / 10 * 2.8),
                          sStressStrategyResume);

        ReportElements::drawWidget(painter, wgtDiag,
                                   static_cast<int>(paper.width() * 0.6), static_cast<int>(paper.height() * 0.6),
                                   paper.x() + paper.width() / 10 * 2, static_cast<int>(paper.y() + paper.height() / 10 * 3.2));
    }

    //! Нижний колонтитул
    QRect rectFooter(paper.x() + paper.width() / 20,
                     paper.y() + paper.height() - static_cast<int>(paper.height() / 30 * 1.5),
                     paper.width() / 20 * 18,
                     static_cast<int>(paper.height() / 30 * 1.5));
    ReportElements::drawFooter(painter, testUid, rectFooter);

    painter->end();
}

