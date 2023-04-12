#include "stressstrategywidget.h"
#include "ui_stressstrategywidget.h"

#include "aanalyserapplication.h"
#include "stressstrategycalculator.h"
#include "vectorfactors.h"
#include "dynamicdiagram.h"
#include "reportelements.h"
#include "stabtestvisualize.h"

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
    m_valTarget = calculator->factors(0)->factorValue(VectorFactorsDefines::KFRUid);
    m_valZones = calculator->factors(1)->factorValue(VectorFactorsDefines::KFRUid);
    m_sTarget = QString(tr("Мишень") + " %1 " + tr("%")).arg(QString::number(m_valTarget, 'f', 0));
    m_sFiveZones = QString(tr("С выделенной зоной") + " %1 " + tr("%")).arg(QString::number(m_valZones, 'f', 0));
    ui->lblTarget->setText(m_sTarget);
    ui->lblFiveZones->setText(m_sFiveZones);

    m_sStressStrategyResume = "";
    if (m_valTarget > m_valZones)
        m_sStressStrategyResume = tr("Качество выполнения пробы мишень выше, чем качество выполнения пробы с выделенной зоной");
    else
    if (m_valTarget < m_valZones)
        m_sStressStrategyResume = tr("Качество выполнения пробы с выделенной зоной выше, чем качество выполнения пробы мишень");
    else
    if (fabs(m_valTarget - m_valZones) < 2)
        m_sStressStrategyResume = tr("Качество выполнения обеих проб одинаково");

    ui->lblStressStrategyResume->setText(m_sStressStrategyResume);

    ui->wgtDiag->clear();
    ui->wgtDiag->setVolume(DynamicDiagramDefines::Volume3D);
    ui->wgtDiag->setKind(DynamicDiagramDefines::KindBar);
    auto itemTarget = new DiagItem(m_valTarget, tr("Мишень"));
    ui->wgtDiag->appendItem(itemTarget);
    auto itemZones = new DiagItem(m_valZones, tr("С выделенной зоной"));
    ui->wgtDiag->appendItem(itemZones);

    m_wgtDiag = ui->wgtDiag;
}

void StressStrategyWidget::print(QPrinter *printer, const QString &testUid)
{
    QPainter *painter = new QPainter(printer);
    QRect paper = printer->pageRect();

    //! Получаем указатель на элземпляр визуализатора
    auto vis = static_cast<AAnalyserApplication*>(QCoreApplication::instance())->getOpenedTest(testUid);
    StabTestVisualize* visualPanel = static_cast<StabTestVisualize*>(vis);
    StressStrategyWidget* visual = static_cast<StressStrategyWidget*>(visualPanel->widget());

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
                          visual->m_sTarget);
        painter->drawText(paper.x() + paper.width() / 12,
                          static_cast<int>(paper.y() + paper.height() / 10 * 2.2),
                          visual->m_sFiveZones);
        painter->drawText(paper.x() + paper.width() / 12,
                          static_cast<int>(paper.y() + paper.height() / 10 * 2.5),
                          visual->m_sStressStrategyResume);

        auto rect = QRect(paper.x() + paper.width() / 12, paper.y() + paper.height() / 10 * 3,
                          static_cast<int>(paper.width() * 0.85), static_cast<int>(paper.height() * 0.6));
        auto ratio = ReportElements::ratio(paper, visual->m_wgtDiag, 2);
        auto bars = QVector<double>() << visual->m_valTarget << visual->m_valZones;
        auto labels = QStringList() << tr("Мишень") << tr("С выделенной зоной");
        ReportElements::drawDynamicDiag(painter, rect, ratio, bars, labels, tr("Качество выполнения проб"),
                                        DynamicDiagramDefines::KindBar, DynamicDiagramDefines::Volume3D);
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
                          visual->m_sTarget);
        painter->drawText(paper.x() + paper.width() / 12,
                          static_cast<int>(paper.y() + paper.height() / 10 * 2.3),
                          visual->m_sFiveZones);
        painter->setFont(QFont("Sans", 14, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 12,
                          static_cast<int>(paper.y() + paper.height() / 10 * 2.8),
                          visual->m_sStressStrategyResume);

        auto rect = QRect(paper.x() + paper.width() / 10 * 2, static_cast<int>(paper.y() + paper.height() / 10 * 3.2),
                          static_cast<int>(paper.width() * 0.6), static_cast<int>(paper.height() * 0.6));
        auto ratio = ReportElements::ratio(paper, visual->m_wgtDiag, 2);
        auto bars = QVector<double>() << visual->m_valTarget << visual->m_valZones;
        auto labels = QStringList() << tr("Мишень") << tr("С выделенной зоной");
        ReportElements::drawDynamicDiag(painter, rect, ratio, bars, labels, tr("Качество выполнения проб"),
                                        DynamicDiagramDefines::KindBar, DynamicDiagramDefines::Volume3D);
    }

    //! Нижний колонтитул
    QRect rectFooter(paper.x() + paper.width() / 20,
                     paper.y() + paper.height() - static_cast<int>(paper.height() / 30 * 1.5),
                     paper.width() / 20 * 18,
                     static_cast<int>(paper.height() / 30 * 1.5));
    ReportElements::drawFooter(painter, testUid, rectFooter);

    painter->end();
}

