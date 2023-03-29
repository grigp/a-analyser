#include "octaedronvisualize.h"
#include "ui_octaedronvisualize.h"

#include <QDebug>

#include "basedefines.h"
#include "octaedroncalculator.h"
#include "reportelements.h"
#include "octaedronpainter.h"

namespace
{
DiagOctaedron *wgtDiag {nullptr};
QString sAverageQuality {""};
OctaedronCalculator* calculator {nullptr};
}

OctaedronVisualize::OctaedronVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OctaedronVisualize)
{
    ui->setupUi(this);
}

OctaedronVisualize::~OctaedronVisualize()
{
    delete ui;
}

void OctaedronVisualize::setTest(const QString &testUid)
{
    if (!m_calculator)
    {
        m_calculator = new OctaedronCalculator(testUid, this);
        m_calculator->calculate();
        calculator = m_calculator;

        auto crm = BaseDefines::CirceRoundRuleModeValueIndex.value(m_calculator->circeRoundRuleMode());
        ui->wgtDiag->setCirceRoundRuleMode(crm);
        auto dm = BaseDefines::DirectionModeValueIndex.value(m_calculator->directionMode());
        ui->wgtDiag->setDirection(dm);

        for (int i = 0; i < 8; ++i)
            ui->wgtDiag->setData(i, static_cast<int>(m_calculator->getValue(i)));

        ui->wgtDiag->doUpdate();

        sAverageQuality = tr("Среднее качество выполнения задания") + " " +
                QString::number(m_calculator->getAverageValue(), 'f', 0) + " " + tr("%");
        ui->lblAverageQuality->setText(sAverageQuality);
        ui->lblAverageQuality->setStyleSheet("font-size: 18pt;");

        wgtDiag = ui->wgtDiag;
    }
}

void OctaedronVisualize::print(QPrinter *printer, const QString &testUid)
{
    QPainter *painter = new QPainter(printer);
    QRect paper = printer->pageRect();

    painter->begin(printer);
    //! Заголовок
    QRect rectHeader(paper.x() + paper.width() / 20, paper.y() + paper.height() / 30, paper.width() / 20 * 18, paper.height() / 30 * 3);
    ReportElements::drawHeader(painter, testUid, rectHeader);

    if (printer->orientation() == QPrinter::Portrait)
    {
        //! Диаграмма
        auto rectDiag = QRect(static_cast<int>(paper.x() + paper.width() * 0.1),
                              static_cast<int>(paper.y() + paper.height() * 0.2),
                              static_cast<int>(paper.width() * 0.8), static_cast<int>(paper.width() * 0.8));
        double ratio = ReportElements::ratio(paper, wgtDiag, 3);
        OctaedronPainter cp(painter, rectDiag);
        auto crm = BaseDefines::CirceRoundRuleModeValueIndex.value(calculator->circeRoundRuleMode());
        cp.setCirceRoundRuleMode(crm);
        auto dm = BaseDefines::DirectionModeValueIndex.value(calculator->directionMode());
        cp.setDirection(dm);
        for (int i = 0; i < 8; ++i)
            cp.setData(i, static_cast<int>(calculator->getValue(i)));
        cp.doPaint(ratio);

        painter->setFont(QFont("Sans", 14, QFont::Bold, false));
        painter->setPen(Qt::darkCyan);
        painter->drawText(paper.x() + paper.width() / 10, static_cast<int>(paper.y() + paper.height() / 16 * 13), sAverageQuality);
    }
    else
    if (printer->orientation() == QPrinter::Landscape)
    {
        //! Диаграмма
        auto rectDiag = QRect(static_cast<int>(paper.x() + paper.width()/5), paper.y() + paper.height()/7,
                              static_cast<int>(paper.height() * 0.8), static_cast<int>(paper.height() * 0.8));
        double ratio = ReportElements::ratio(paper, wgtDiag, 3);
        OctaedronPainter cp(painter, rectDiag);
        auto crm = BaseDefines::CirceRoundRuleModeValueIndex.value(calculator->circeRoundRuleMode());
        cp.setCirceRoundRuleMode(crm);
        auto dm = BaseDefines::DirectionModeValueIndex.value(calculator->directionMode());
        cp.setDirection(dm);
        for (int i = 0; i < 8; ++i)
            cp.setData(i, static_cast<int>(calculator->getValue(i)));
        cp.doPaint(ratio);

        painter->setFont(QFont("Sans", 14, QFont::Bold, false));
        painter->setPen(Qt::darkCyan);
        painter->drawText(paper.x() + paper.width() / 5, static_cast<int>(paper.y() + paper.height() / 16 * 14), sAverageQuality);
    }

    //! Нижний колонтитул
    QRect rectFooter(paper.x() + paper.width() / 20,
                     paper.y() + paper.height() - static_cast<int>(paper.height() / 30 * 1.5),
                     paper.width() / 20 * 18,
                     static_cast<int>(paper.height() / 30 * 1.5));
    ReportElements::drawFooter(painter, testUid, rectFooter);

    painter->end();
}
