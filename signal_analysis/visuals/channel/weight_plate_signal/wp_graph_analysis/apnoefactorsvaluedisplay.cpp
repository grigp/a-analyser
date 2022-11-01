#include "apnoefactorsvaluedisplay.h"
#include "ui_apnoefactorsvaluedisplay.h"

ApnoeFactorsValueDisplay::ApnoeFactorsValueDisplay(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ApnoeFactorsValueDisplay)
{
    ui->setupUi(this);
}

ApnoeFactorsValueDisplay::~ApnoeFactorsValueDisplay()
{
    delete ui;
}

void ApnoeFactorsValueDisplay::assignValues(const int count, const double timeAverage, const double timeMax)
{
    ui->lblApnoeFactsCount->setText(tr("Количество приступов") + " - " + QString::number(count));
    ui->lblApnoeFactTimeAverage->setText(tr("Средняя длительность приступа") + " - " + QString::number(timeAverage) + " " + tr("сек"));
    ui->lblApnoeFactTimeMax->setText(tr("Максимальная длительность приступа") + " - " + QString::number(timeMax) + " " + tr("сек"));
}
