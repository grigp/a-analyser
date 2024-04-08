#include "babyposturewidget.h"
#include "ui_babyposturewidget.h"

#include "babyposturecalculator.h"

BabyPostureWidget::BabyPostureWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BabyPostureWidget)
{
    ui->setupUi(this);
}

BabyPostureWidget::~BabyPostureWidget()
{
    delete ui;
}

void BabyPostureWidget::calculate(BabyPostureCalculator *calculator, const QString &testUid)
{
    Q_UNUSED(testUid);
    ui->teResume->setStyleSheet("font-size: 14pt; color: rgb(32,88,103);");

    for (int i = 0; i < calculator->factorsCount(); ++i)
    {
        auto factor = calculator->factor(i);
        ui->teResume->append(factor.name + "  " + factor.shortName + " : " + factor.valueFmt);
    }
}

void BabyPostureWidget::print(QPrinter *printer, const QString &testUid)
{

}

void BabyPostureWidget::paintPreview(QPainter *painter, QRect &rect, const QString &testUid, TestCalculator *calculator)
{

}
