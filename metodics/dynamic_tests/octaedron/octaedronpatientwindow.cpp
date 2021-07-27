#include "octaedronpatientwindow.h"
#include "ui_octaedronpatientwindow.h"

OctaedronPatientWindow::OctaedronPatientWindow(const int diap, QWidget *parent) :
    StabDynamicTestPatientWindow(parent),
    ui(new Ui::OctaedronPatientWindow)
  , m_diap(diap)
{
    ui->setupUi(this);

    setVisibleMarker(true);
//    addTarget(0, 0, Qt::green, 30);
}

OctaedronPatientWindow::~OctaedronPatientWindow()
{
    delete ui;
}

void OctaedronPatientWindow::setMarker(const double x, const double y)
{
    StabDynamicTestPatientWindow::setMarker(x, y);
}

void OctaedronPatientWindow::run()
{
    StabDynamicTestPatientWindow::run();
}

void OctaedronPatientWindow::stop()
{
    StabDynamicTestPatientWindow::stop();
}

void OctaedronPatientWindow::setDiap(const int diap)
{
    Q_UNUSED(diap);
    StabDynamicTestPatientWindow::setDiap(m_diap);
}
