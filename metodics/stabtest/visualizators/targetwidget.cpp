#include "targetwidget.h"
#include "ui_targetwidget.h"

#include "aanalyserapplication.h"
#include "resultinfo.h"
#include "channelsdefines.h"
#include "targetcalculator.h"
#include "targetfactors.h"
#include "skgwidget.h"
#include "dynamicdiagram.h"
#include "testresultdata.h"
#include "reportelements.h"
#include "datadefines.h"
#include "dataprovider.h"
#include "stabtestvisualize.h"

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
    m_sScore = QString(tr("Количество очков") + " %1").arg(QString::number(val, 'f', 0));
    m_sTotalScore = QString(tr("Суммарный балл") + " %1").arg(QString::number(valT, 'f', 0));
    ui->lblScore->setText(m_sScore);
    ui->lblTotalScore->setText(m_sTotalScore);
    m_calculator = calculator;

    showSKG(calculator, testUid);
    showDiagram(calculator, testUid);

    connect(ui->wgtDiagram, &DynamicDiagram::selectItem, this, &TargetWidget::on_selectItem);
}

void TargetWidget::print(QPrinter *printer, const QString &testUid)
{
    QPainter *painter = new QPainter(printer);
    QRect paper = printer->pageRect();

    //! Получаем указатель на элземпляр визуализатора
    auto vis = static_cast<AAnalyserApplication*>(QCoreApplication::instance())->getOpenedTest(testUid);
    StabTestVisualize* visualPanel = static_cast<StabTestVisualize*>(vis);
    TargetWidget* visual = static_cast<TargetWidget*>(visualPanel->widget());

    painter->begin(printer);
    //! Заголовок
    QRect rectHeader(paper.x() + paper.width() / 20, paper.y() + paper.height() / 30, paper.width() / 20 * 18, paper.height() / 30 * 3);
    ReportElements::drawHeader(painter, testUid, rectHeader);

    if (printer->orientation() == QPrinter::Portrait)
    {
        painter->setFont(QFont("Sans", 16, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 10,
                          static_cast<int>(paper.y() + paper.height() / 10 * 1.6),
                          visual->m_sScore);
        painter->setFont(QFont("Sans", 12, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 10,
                          static_cast<int>(paper.y() + paper.height() / 10 * 1.8),
                          visual->m_sTotalScore);

        auto rect = QRect(paper.x() + paper.width() / 10, paper.y() + paper.height() / 10 * 2,
                          static_cast<int>(paper.width() * 0.85), static_cast<int>(paper.height() * 0.28));
        auto ratio = ReportElements::ratio(paper, visual->m_wgtDiagram, 2);
        QVector<double> bars;
        QStringList labels;
        for (int i = 9; i >= 0; --i)
        {
            bars << visual->m_calculator->factors()->histogram(i);
            labels << QString::number(i + 1);
        }
        ReportElements::drawDynamicDiag(painter, rect, ratio, bars, labels, tr("Процент времени пребывания в зонах"),
                                        DynamicDiagramDefines::KindBar, DynamicDiagramDefines::Volume3D);

        //! СКГ
        auto rectSKG = QRect(paper.x() + paper.width() / 2 - static_cast<int>(paper.width() * 0.33),
                             static_cast<int>(paper.y() + paper.height() / 10 * 5.1),
                             static_cast<int>(paper.width() * 0.65),
                             static_cast<int>(paper.height() * 0.42));
        ratio = ReportElements::ratio(paper, visual->m_wgtSKG, 5);
        if (DataProvider::channelExists(testUid, 0, ChannelsDefines::chanStab))
            ReportElements::drawSKG(painter, rectSKG, testUid, 0, ratio);
    }
    else
    if (printer->orientation() == QPrinter::Landscape)
    {
        painter->setFont(QFont("Sans", 16, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 10,
                          static_cast<int>(paper.y() + paper.height() / 10 * 2),
                          visual->m_sScore);
        painter->setFont(QFont("Sans", 12, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 10,
                          static_cast<int>(paper.y() + paper.height() / 10 * 2.4),
                          visual->m_sTotalScore);

        auto rect = QRect(paper.x() + paper.width() / 15, static_cast<int>(paper.y() + paper.height() * 0.28),
                          static_cast<int>(paper.width() * 0.6), static_cast<int>(paper.height() * 0.5));
        auto ratio = ReportElements::ratio(paper, visual->m_wgtDiagram, 2);
        QVector<double> bars;
        QStringList labels;
        for (int i = 9; i >= 0; --i)
        {
            bars << visual->m_calculator->factors()->histogram(i);
            labels << QString::number(i + 1);
        }
        ReportElements::drawDynamicDiag(painter, rect, ratio, bars, labels, tr("Процент времени пребывания в зонах"),
                                        DynamicDiagramDefines::KindBar, DynamicDiagramDefines::Volume3D);
        //! СКГ
        auto rectSKG = QRect(static_cast<int>(paper.x() + paper.width() / 10 * 6.2), paper.y() + paper.height() / 10 * 3,
                             static_cast<int>(paper.width() * 0.4), static_cast<int>(paper.height() * 0.4));
        ratio = ReportElements::ratio(paper, visual->m_wgtSKG, 5);
        if (DataProvider::channelExists(testUid, 0, ChannelsDefines::chanStab))
            ReportElements::drawSKG(painter, rectSKG, testUid, 0, ratio);
    }

    //! Нижний колонтитул
    QRect rectFooter(paper.x() + paper.width() / 20,
                     paper.y() + paper.height() - static_cast<int>(paper.height() / 30 * 1.5),
                     paper.width() / 20 * 18,
                     static_cast<int>(paper.height() / 30 * 1.5));
    ReportElements::drawFooter(painter, testUid, rectFooter);

    painter->end();
}

void TargetWidget::paintPreview(QPainter *painter, QRect &rect, const QString &testUid, TestCalculator *calculator)
{
    Q_UNUSED(testUid);
    TargetCalculator* calc = static_cast<TargetCalculator*>(calculator);
    auto val = calc->factors()->factorValue(TargetFactorsDefines::TargetScoreUid);
    auto valT = calc->factors()->factorValue(TargetFactorsDefines::TargetTotalScoreUid);
    auto sScore = QString(tr("Количество очков") + " %1").arg(QString::number(val, 'f', 0));
    auto sTotalScore = QString(tr("Суммарный балл") + " %1").arg(QString::number(valT, 'f', 0));

    painter->save();

    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    painter->setFont(QFont("Sans", 7, QFont::Bold, false));

    painter->drawText(rect.x() + 5, rect.y() + 5, sScore);
    painter->drawText(rect.x() + 5, rect.y() + 15, sTotalScore);

    painter->restore();
}

void TargetWidget::timerEvent(QTimerEvent *event)
{
    int diap = 1;
    int step = 0;
    while (diap < m_maxValue)
    {
        diap = diap * 2;
        ++step;
    }
    ui->wgtSKG->setDiap(diap);

    QWidget::timerEvent(event);
}

void TargetWidget::on_selectItem(const int idx)
{
    if (idx >= 0 && idx < ui->wgtDiagram->itemCount() - 1)
    {
        int v = m_calculator->factors()->histogram(9 - idx);
        ui->wgtDiagram->setTitle(tr("Процент времени пребывания в зонах") + " (" + QString::number(v) + " %)");
    }
}

void TargetWidget::showSKG(TargetCalculator *calculator, const QString &testUid)
{
    Q_UNUSED(calculator);
    auto* trd = new TestResultData();
    trd->openTest(testUid);
    Q_ASSERT(trd->probesCount() == 1);

    auto* probe = trd->probe(0);

    auto *sig = probe->signal(ChannelsDefines::chanStab);
    ui->wgtSKG->setSignal(sig);
    m_maxValue = sig->absMaxValue();
    ui->wgtSKG->setVisibleMarker(false);

    m_tmSetDiap = startTimer(30);
    m_wgtSKG = ui->wgtSKG;

//    auto angle = calculator->factors()->ellipse().angle;
//    auto sizeA = calculator->classicFactors(i)->ellipse().sizeA;
//    auto sizeB = calculator->classicFactors(i)->ellipse().sizeB;
    //    ui->wgtSKG->setEllipse(sizeA, sizeB, angle);
}

void TargetWidget::showDiagram(TargetCalculator *calculator, const QString &testUid)
{
    Q_UNUSED(testUid);
    ui->wgtDiagram->setKind(DynamicDiagramDefines::KindBar);
    ui->wgtDiagram->setVolume(DynamicDiagramDefines::Volume3D);
    ui->wgtDiagram->setTitle(tr("Процент времени пребывания в зонах"));
    ui->wgtDiagram->setBottomText(tr("Зоны"));
    for (int i = 9; i >= 0; --i)
    {
        auto item = new DiagItem(calculator->factors()->histogram(i), QString::number(i + 1));
        ui->wgtDiagram->appendItem(item);
    }
    m_wgtDiagram = ui->wgtDiagram;
}
