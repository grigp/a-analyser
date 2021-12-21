#include "trianglevisualize.h"
#include "ui_trianglevisualize.h"

#include <QPainter>

#include "settingsprovider.h"
#include "trianglecalculator.h"
#include "reportelements.h"

TriangleVisualize::TriangleVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TriangleVisualize)
{
    ui->setupUi(this);

    auto val = SettingsProvider::valueFromRegAppCopy("TriangleResultWidget", "CurrentPage").toInt();
    ui->twPages->setCurrentIndex(val);
}

TriangleVisualize::~TriangleVisualize()
{
    delete ui;
}

void TriangleVisualize::setTest(const QString &testUid)
{
    if (!m_calculator)
    {
        m_calculator = new TriangleCalculator(testUid, this);
        m_calculator->calculate();
    }
}

void TriangleVisualize::print(QPrinter *printer, const QString &testUid)
{
    QPainter *painter = new QPainter(printer);
    QRect paper = printer->pageRect();

    painter->begin(printer);
    //! Заголовок
    QRect rectHeader(paper.x() + paper.width() / 20, paper.y() + paper.height() / 30, paper.width() / 20 * 18, paper.height() / 30 * 3);
    ReportElements::drawHeader(painter, testUid, rectHeader);

    if (printer->orientation() == QPrinter::Portrait)
    {
//        //! Диаграмма. Копируется из виджета
//        ReportElements::drawWidget(painter, wgtDiag,
//                                   static_cast<int>(paper.width() * 0.85), static_cast<int>(paper.height() * 0.85),
//                                   paper.x() + paper.width()/10, static_cast<int>(paper.y() + paper.height()/4));
    }
    else
    if (printer->orientation() == QPrinter::Landscape)
    {
//        //! Диаграмма. Копируется из виджета
//        ReportElements::drawWidget(painter, wgtDiag,
//                                   static_cast<int>(paper.width() * 0.75), static_cast<int>(paper.height() * 0.75),
//                                   static_cast<int>(paper.x() + paper.width()/5.5), paper.y() + paper.height()/6);
    }

    //! Нижний колонтитул
    QRect rectFooter(paper.x() + paper.width() / 20,
                     paper.y() + paper.height() - static_cast<int>(paper.height() / 30 * 1.5),
                     paper.width() / 20 * 18,
                     static_cast<int>(paper.height() / 30 * 1.5));
    ReportElements::drawFooter(painter, testUid, rectFooter);

    painter->end();
}

void TriangleVisualize::curPageChanged(int pageIdx)
{
    SettingsProvider::setValueToRegAppCopy("TriangleResultWidget", "CurrentPage", pageIdx);
}

