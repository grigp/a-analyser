#include "lineswindow.h"
#include "ui_lineswindow.h"

#include <QVariant>

LinesWindow::LinesWindow(QWidget *parent) :
    PatientWindow(parent),
    ui(new Ui::LinesWindow)
{
    ui->setupUi(this);
}

LinesWindow::~LinesWindow()
{
    delete ui;
}

void LinesWindow::setDiap(const int diap)
{

}

void LinesWindow::setMarker(const double x, const double y)
{

}

void LinesWindow::run()
{

}

void LinesWindow::stop()
{

}

void LinesWindow::setFrontComment(const QString &comment)
{

}

QVariant LinesWindow::result()
{

}
