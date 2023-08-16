#include "targetwindow.h"
#include "ui_targetwindow.h"

#include "metodicdefines.h"

TargetWindow::TargetWindow(QWidget *parent) :
    PatientWindow(parent),
    ui(new Ui::TargetWindow)
{
    ui->setupUi(this);

    ui->lblFrontComment->setStyleSheet(MetodicDefines::AutoModeMessageStylePatient);
    setFrontComment("");
}

TargetWindow::~TargetWindow()
{
    delete ui;
}

void TargetWindow::setDiap(const int diap)
{
    ui->wgtTarget->setDiap(diap);
}

void TargetWindow::setMarker(const double x, const double y)
{
    ui->wgtTarget->setMarker(x, y);
}

void TargetWindow::run()
{

}

void TargetWindow::stop()
{

}

void TargetWindow::setFrontComment(const QString &comment)
{
    ui->lblFrontComment->setText(comment);
    ui->lblFrontComment->setVisible(comment != "");
}

QVariant TargetWindow::result()
{
    return QVariant();
}

void TargetWindow::resizeEvent(QResizeEvent *event)
{
    ui->lblFrontComment->setGeometry(ui->wgtTarget->geometry());
    PatientWindow::resizeEvent(event);
}
