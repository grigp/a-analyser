#include "mwcompetitivestickvisualize.h"
#include "ui_mwcompetitivestickvisualize.h"

#include "mwcompetitivestickcalculator.h"

MWCompetitiveStickVisualize::MWCompetitiveStickVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MWCompetitiveStickVisualize)
{
    ui->setupUi(this);
}

MWCompetitiveStickVisualize::~MWCompetitiveStickVisualize()
{
    delete ui;
}

void MWCompetitiveStickVisualize::setTest(const QString &testUid)
{
    if (!m_calculator)
    {
        m_calculator = new MWCompetitiveStickCalculator(testUid, this);
        m_calculator->calculate();
    }
}

void MWCompetitiveStickVisualize::print(QPrinter *printer, const QString &testUid)
{

}

void MWCompetitiveStickVisualize::paintPreview(QPainter *painter, QRect &rect, const QString &testUid, MWCompetitiveStickCalculator *calculator)
{

}
