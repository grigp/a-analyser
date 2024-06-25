#include "octaedronpatientwindow.h"
#include "ui_octaedronpatientwindow.h"

#include <QTimer>
#include <QDebug>

OctaedronPatientWindow::OctaedronPatientWindow(const int diap,  QList<BaseDefines::TargetInfo> targets, QWidget *parent) :
    StabDynamicTestPatientWindow(parent),
    ui(new Ui::OctaedronPatientWindow)
  , m_diap(diap)
  , m_targets(targets)
{
    ui->setupUi(this);

    setVisibleMarker(true);
    createTargets();
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

void OctaedronPatientWindow::createTargets()
{
    foreach (auto target, m_targets)
    {
        addTarget(target.x, target.y, Qt::gray, 30, target.visible);
    }
    addTarget(0, 0, Qt::green, 30);
}
