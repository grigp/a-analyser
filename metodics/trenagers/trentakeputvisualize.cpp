#include "trentakeputvisualize.h"
#include "ui_trentakeputvisualize.h"

#include "trentakeputtestcalculator.h"

TrenTakePutVisualize::TrenTakePutVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrenTakePutVisualize)
{
    ui->setupUi(this);
}

TrenTakePutVisualize::~TrenTakePutVisualize()
{
    if (m_calculator)
        delete m_calculator;
    delete ui;
}

void TrenTakePutVisualize::setTest(const QString &testUid)
{
    if (!m_calculator)
        m_calculator = new TrenTakePutTestCalculator(testUid);

    m_calculator->calculate();

    if (m_calculator->factorsCount() >= 2)
    {
        ui->lblScore->setText(QString::number(m_calculator->factor(0).value()));
        ui->lblFaults->setText(QString::number(m_calculator->factor(1).value()));
    }

}
