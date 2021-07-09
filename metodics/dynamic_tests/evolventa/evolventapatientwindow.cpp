#include "evolventapatientwindow.h"
#include "ui_evolventapatientwindow.h"

#include <QDebug>

EvolventaPatientWindow::EvolventaPatientWindow(const int diap, QWidget *parent) :
    StabDynamicTestPatientWindow(parent),
    ui(new Ui::EvolventaPatientWindow)
  , m_diap(diap)
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

void EvolventaPatientWindow::setDiap(const int diap)
{
    Q_UNUSED(diap);
    StabDynamicTestPatientWindow::setDiap(m_diap);
}

