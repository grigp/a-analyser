#include "coloredcirclewindow.h"
#include "ui_coloredcirclewindow.h"

namespace  {

static QList<QColor> colors { Qt::white, Qt::red, Qt::green, Qt::blue, Qt::darkYellow, Qt::darkGray};

}


ColoredCircleWindow::ColoredCircleWindow(QWidget *parent) :
    PatientWindow(parent),
    ui(new Ui::ColoredCircleWindow)
{
    ui->setupUi(this);
}

ColoredCircleWindow::~ColoredCircleWindow()
{
    delete ui;
}

void ColoredCircleWindow::setStabData(const double x, const double y)
{

}

void ColoredCircleWindow::run()
{
    m_tm = startTimer(1000);

}

void ColoredCircleWindow::stop()
{
    killTimer(m_tm);
}

void ColoredCircleWindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_tm)
        nextColor();
}

void ColoredCircleWindow::nextColor()
{
    int rnd = qrand() % colors.size();
    ui->wgtCircle->setColor(colors.at(rnd));
}
