#include "coloredcirclewindow.h"
#include "ui_coloredcirclewindow.h"

#include <QResizeEvent>
#include <QDebug>

#include "metodicdefines.h"

namespace  {

static QList<QColor> colors {Qt::white, Qt::red, Qt::green, Qt::blue, Qt::darkYellow, Qt::lightGray};

}


ColoredCircleWindow::ColoredCircleWindow(QWidget *parent) :
    PatientWindow(parent),
    ui(new Ui::ColoredCircleWindow)
{
    ui->setupUi(this);
    nextColor();

    ui->lblFrontComment->setStyleSheet(MetodicDefines::AutoModeMessageStylePatient);
    setFrontComment("");
}

ColoredCircleWindow::~ColoredCircleWindow()
{
    delete ui;
}

void ColoredCircleWindow::setParams(const QJsonObject &params)
{
    Q_UNUSED(params);
}

void ColoredCircleWindow::setDiap(const int diap)
{
    Q_UNUSED(diap);
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

void ColoredCircleWindow::setFrontComment(const QString &comment)
{
    ui->lblFrontComment->setText(comment);
    ui->lblFrontComment->setVisible(comment != "");
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

void ColoredCircleWindow::resizeEvent(QResizeEvent *event)
{
    ui->lblFrontComment->setGeometry(ui->wgtCircle->geometry());
    PatientWindow::resizeEvent(event);
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
