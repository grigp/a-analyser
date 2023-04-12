#include "statechampionswidget.h"
#include "ui_statechampionswidget.h"

#include "statechampionstestcalculator.h"
#include "aanalyserapplication.h"
#include "vectorfactors.h"
#include "classicfactors.h"
#include "ratioprobesfactors.h"
#include "reportelements.h"
#include "stabtestvisualize.h"

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

    m_sSquareProbe1 = QString::number(calculator->square(0), 'f', fmtS);
    m_sSquareProbe2 = QString::number(calculator->square(1), 'f', fmtS);
    m_sSquareProbe3 = QString::number(calculator->square(2), 'f', fmtS);
    m_sSpeedProbe1 = QString::number(calculator->speed(0), 'f', fmtV);
    m_sSpeedProbe2 = QString::number(calculator->speed(1), 'f', fmtV);
    m_sSpeedProbe3 = QString::number(calculator->speed(2), 'f', fmtV);
    m_sKFRProbe3 = QString::number(calculator->targetKFR(), 'f', fmtKFR);
    m_sRombKoefValue = QString::number(calculator->rombKoef(), 'f', fmtRK);
    ui->lblSquareProbe1->setText(m_sSquareProbe1);
    ui->lblSquareProbe2->setText(m_sSquareProbe2);
    ui->lblSquareProbe3->setText(m_sSquareProbe3);
    ui->lblSpeedProbe1->setText(m_sSpeedProbe1);
    ui->lblSpeedProbe2->setText(m_sSpeedProbe2);
    ui->lblSpeedProbe3->setText(m_sSpeedProbe3);
    ui->lblKFRProbe3->setText(m_sKFRProbe3);
    ui->lblRombKoefValue->setText(m_sRombKoefValue);
}

void StateChampionsWidget::print(QPrinter *printer, const QString &testUid)
{
    QPainter *painter = new QPainter(printer);
    QRect paper = printer->pageRect();

    //! Получаем указатель на экземпляр визуализатора
    auto vis = static_cast<AAnalyserApplication*>(QCoreApplication::instance())->getOpenedTest(testUid);
    StabTestVisualize* visualPanel = static_cast<StabTestVisualize*>(vis);
    StateChampionsWidget* visual = static_cast<StateChampionsWidget*>(visualPanel->widget());

    painter->begin(printer);
    //! Заголовок
    QRect rectHeader(paper.x() + paper.width() / 20, paper.y() + paper.height() / 30, paper.width() / 20 * 18, paper.height() / 30 * 3);
    ReportElements::drawHeader(painter, testUid, rectHeader);

    auto drawTable = [&]()
    {
        painter->setFont(QFont("Sans", 12, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 10 * 4,
                          static_cast<int>(paper.y() + paper.height() / 10 * 3),
                          tr("Открытые глаза"));
        painter->drawText(paper.x() + paper.width() / 10 * 6,
                          static_cast<int>(paper.y() + paper.height() / 10 * 3),
                          tr("Закрытые глаза"));
        painter->drawText(paper.x() + paper.width() / 10 * 8,
                          static_cast<int>(paper.y() + paper.height() / 10 * 3),
                          tr("Мишень"));

        painter->drawText(paper.x() + paper.width() / 10,
                          static_cast<int>(paper.y() + paper.height() / 10 * 4),
                          tr("Средняя скорость ЦД"));
        painter->setFont(QFont("Sans", 18, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 10 * 4,
                          static_cast<int>(paper.y() + paper.height() / 10 * 4),
                          visual->m_sSpeedProbe1);
        painter->drawText(paper.x() + paper.width() / 10 * 6,
                          static_cast<int>(paper.y() + paper.height() / 10 * 4),
                          visual->m_sSpeedProbe2);
        painter->drawText(paper.x() + paper.width() / 10 * 8,
                          static_cast<int>(paper.y() + paper.height() / 10 * 4),
                          visual->m_sSpeedProbe3);

        painter->setFont(QFont("Sans", 12, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 10,
                          static_cast<int>(paper.y() + paper.height() / 10 * 5),
                          tr("Площадь эллипса"));
        painter->setFont(QFont("Sans", 18, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 10 * 4,
                          static_cast<int>(paper.y() + paper.height() / 10 * 5),
                          visual->m_sSquareProbe1);
        painter->drawText(paper.x() + paper.width() / 10 * 6,
                          static_cast<int>(paper.y() + paper.height() / 10 * 5),
                          visual->m_sSquareProbe2);
        painter->drawText(paper.x() + paper.width() / 10 * 8,
                          static_cast<int>(paper.y() + paper.height() / 10 * 5),
                          visual->m_sSquareProbe3);

        painter->setFont(QFont("Sans", 12, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 10,
                          static_cast<int>(paper.y() + paper.height() / 10 * 6),
                          tr("КФР"));
        painter->setFont(QFont("Sans", 18, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 10 * 8,
                          static_cast<int>(paper.y() + paper.height() / 10 * 6),
                          visual->m_sKFRProbe3);

        painter->setFont(QFont("Sans", 12, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 10,
                          static_cast<int>(paper.y() + paper.height() / 10 * 7),
                          tr("Коэффициент Ромберга"));
        painter->setFont(QFont("Sans", 18, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 10 * 4,
                          static_cast<int>(paper.y() + paper.height() / 10 * 7),
                          visual->m_sRombKoefValue);
    };


    if (printer->orientation() == QPrinter::Portrait)
    {
        drawTable();
    }
    else
    if (printer->orientation() == QPrinter::Landscape)
    {
        drawTable();
    }

    //! Нижний колонтитул
    QRect rectFooter(paper.x() + paper.width() / 20,
                     paper.y() + paper.height() - static_cast<int>(paper.height() / 30 * 1.5),
                     paper.width() / 20 * 18,
                     static_cast<int>(paper.height() / 30 * 1.5));
    ReportElements::drawFooter(painter, testUid, rectFooter);

    painter->end();
}
