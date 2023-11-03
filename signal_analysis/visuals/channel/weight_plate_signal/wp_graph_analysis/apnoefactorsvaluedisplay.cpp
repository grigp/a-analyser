#include "apnoefactorsvaluedisplay.h"
#include "ui_apnoefactorsvaluedisplay.h"

ApnoeFactorsValueDisplay::ApnoeFactorsValueDisplay(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ApnoeFactorsValueDisplay)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    ui->frDetail->setVisible(false);
    adjustSize();
    setFixedSize(size());
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

void ApnoeFactorsValueDisplay::setApnoeList(const QStringList &list)
{
    foreach (auto line, list)
        ui->teApnoeList->append(line);
}

void ApnoeFactorsValueDisplay::setLog(const QStringList &log)
{
    foreach (auto line, log)
        ui->teLog->append(line);
}

void ApnoeFactorsValueDisplay::on_showDetail()
{
    ui->frDetail->setVisible(!ui->frDetail->isVisible());
    adjustSize();
    if (!ui->frDetail->isVisible())
        setFixedSize(QSize(800, 120)); //size().height());
    else
        setFixedSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
}

void ApnoeFactorsValueDisplay::on_writeToFile()
{

}
