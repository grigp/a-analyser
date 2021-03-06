#include "stepoffsettpatientwindow.h"
#include "ui_stepoffsettpatientwindow.h"

StepOffsettPatientWindow::StepOffsettPatientWindow(QWidget *parent) :
    StabDynamicTestPatientWindow(parent),
    ui(new Ui::StepOffsettPatientWindow)
{
    ui->setupUi(this);

    setVisibleMarker(true);
    addTarget(0, 0, Qt::green, 30);
}

StepOffsettPatientWindow::~StepOffsettPatientWindow()
{
    delete ui;
}

void StepOffsettPatientWindow::setMarker(const double x, const double y)
{
    StabDynamicTestPatientWindow::setMarker(x, y);
}

void StepOffsettPatientWindow::run()
{
    StabDynamicTestPatientWindow::run();
}

void StepOffsettPatientWindow::stop()
{
    StabDynamicTestPatientWindow::stop();
}

void StepOffsettPatientWindow::setDiap(const int diap)
{
    Q_UNUSED(diap);
    StabDynamicTestPatientWindow::setDiap(m_diap);
}

void StepOffsettPatientWindow::setDiapSpecific(const int diap)
{
    m_diap = diap;
    setDiap(diap);
}
