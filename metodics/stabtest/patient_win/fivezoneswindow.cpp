#include "fivezoneswindow.h"
#include "ui_fivezoneswindow.h"

#include "fivezonearea.h"

FiveZonesWindow::FiveZonesWindow(QWidget *parent) :
    PatientWindow(parent),
    ui(new Ui::FiveZonesWindow)
{
    ui->setupUi(this);
}

FiveZonesWindow::~FiveZonesWindow()
{
    delete ui;
}

void FiveZonesWindow::setDiap(const int diap)
{
    ui->wgtFiveZones->setDiap(diap);
}

void FiveZonesWindow::setMarker(const double x, const double y)
{
    ui->wgtFiveZones->setMarker(x, y);
}

void FiveZonesWindow::run()
{

}

void FiveZonesWindow::stop()
{

}

void FiveZonesWindow::setFrontComment(const QString &comment)
{

}

QVariant FiveZonesWindow::result()
{
    return QVariant();
}
