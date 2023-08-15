#include "targetwindow.h"
#include "ui_targetwindow.h"

TargetWindow::TargetWindow(QWidget *parent) :
    PatientWindow(parent),
    ui(new Ui::TargetWindow)
{
    ui->setupUi(this);
}

TargetWindow::~TargetWindow()
{
    delete ui;
}

void TargetWindow::setDiap(const int diap)
{
    ui->widget->setDiap(diap);
}

void TargetWindow::setMarker(const double x, const double y)
{
    ui->widget->setMarker(x, y);
}

void TargetWindow::run()
{

}

void TargetWindow::stop()
{

}

void TargetWindow::setFrontComment(const QString &comment)
{

}

QVariant TargetWindow::result()
{
    return QVariant();
}
