#include "crosspatientwindow.h"
#include "ui_crosspatientwindow.h"

#include <QDebug>

CrossPatientWindow::CrossPatientWindow(QWidget *parent) :
    StabDynamicTestPatientWindow(parent),
    ui(new Ui::CrossPatientWindow)
{
    ui->setupUi(this);
    setVisibleMarker(true);
    addTarget(0, 0, Qt::green, 30);
}

CrossPatientWindow::~CrossPatientWindow()
{
    delete ui;
}

void CrossPatientWindow::setMarker(const double x, const double y)
{
    StabDynamicTestPatientWindow::setMarker(x, y);
}

void CrossPatientWindow::run()
{
    StabDynamicTestPatientWindow::run();
}

void CrossPatientWindow::stop()
{
    StabDynamicTestPatientWindow::stop();
}
