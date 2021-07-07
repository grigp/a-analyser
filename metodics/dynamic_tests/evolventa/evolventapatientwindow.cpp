#include "evolventapatientwindow.h"
#include "ui_evolventapatientwindow.h"

EvolventaPatientWindow::EvolventaPatientWindow(QWidget *parent) :
    StabDynamicTestPatientWindow(parent),
    ui(new Ui::EvolventaPatientWindow)
{
    ui->setupUi(this);

    setVisibleMarker(true);
    addTarget(0, 0, Qt::green, 30);
}

EvolventaPatientWindow::~EvolventaPatientWindow()
{
    delete ui;
}

void EvolventaPatientWindow::setMarker(const double x, const double y)
{
    StabDynamicTestPatientWindow::setMarker(x, y);
}

void EvolventaPatientWindow::run()
{
    StabDynamicTestPatientWindow::run();
}

void EvolventaPatientWindow::stop()
{
    StabDynamicTestPatientWindow::stop();
}
