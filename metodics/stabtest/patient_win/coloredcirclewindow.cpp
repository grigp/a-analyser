#include "coloredcirclewindow.h"
#include "ui_coloredcirclewindow.h"

namespace  {

static QList<QColor> colors {Qt::white, Qt::red, Qt::green, Qt::blue, Qt::darkYellow, Qt::lightGray};

}


ColoredCircleWindow::ColoredCircleWindow(QWidget *parent) :
    PatientWindow(parent),
    ui(new Ui::ColoredCircleWindow)
{
    ui->setupUi(this);
    nextColor();
}

ColoredCircleWindow::~ColoredCircleWindow()
{
    delete ui;
}

void ColoredCircleWindow::setDiap(const int diap)
{

}

void ColoredCircleWindow::setMarker(const double x, const double y)
{
    Q_UNUSED(x);
    Q_UNUSED(y);
}

void ColoredCircleWindow::run()
{
    m_tm = startTimer(1000);

}

void ColoredCircleWindow::stop()
{
    killTimer(m_tm);
}

QVariant ColoredCircleWindow::result()
{
    return m_targetColorCount;
}

void ColoredCircleWindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_tm)
        nextColor();
}

void ColoredCircleWindow::nextColor()
{
    QColor color;
    do
    {
        int rnd = qrand() % colors.size();
        color = colors.at(rnd);
    }
    while(color == ui->wgtCircle->color());

    if (color == Qt::white)
        ++m_targetColorCount;

    ui->wgtCircle->setColor(color);
}
