#include "soundpickwindow.h"
#include "ui_soundpickwindow.h"

SoundPickWindow::SoundPickWindow(QWidget *parent) :
    PatientWindow(parent),
    ui(new Ui::SoundPickWindow)
{
    ui->setupUi(this);
}

SoundPickWindow::~SoundPickWindow()
{
    delete ui;
}

void SoundPickWindow::setStabData(const double x, const double y)
{

}

void SoundPickWindow::run()
{

}

void SoundPickWindow::stop()
{

}
