#include "crosspatientwindow.h"
#include "ui_crosspatientwindow.h"

#include <QDebug>

CrossPatientWindow::CrossPatientWindow(QWidget *parent) :
    StabDynamicTestPatientWindow(parent),
    ui(new Ui::CrossPatientWindow)
{
    ui->setupUi(this);
}

CrossPatientWindow::~CrossPatientWindow()
{
    delete ui;
}

void CrossPatientWindow::run()
{
}

void CrossPatientWindow::stop()
{
}
