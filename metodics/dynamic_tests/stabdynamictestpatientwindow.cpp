#include "stabdynamictestpatientwindow.h"
#include "ui_stabdynamictestpatientwindow.h"

StabDynamicTestPatientWindow::StabDynamicTestPatientWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StabDynamicTestPatientWindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::darkGray);
    setAutoFillBackground(true);
    setPalette(pal);
}

StabDynamicTestPatientWindow::~StabDynamicTestPatientWindow()
{
    delete ui;
}

void StabDynamicTestPatientWindow::run()
{

}

void StabDynamicTestPatientWindow::stop()
{

}

void StabDynamicTestPatientWindow::setMarker(const double x, const double y)
{
    m_X = x;
    m_Y = y;
}

void StabDynamicTestPatientWindow::setVisibleMarker(const bool isVisible)
{
    m_isVisibleMarker = isVisible;
}
