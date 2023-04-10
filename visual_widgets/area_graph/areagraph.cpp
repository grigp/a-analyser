#include "areagraph.h"
#include "ui_areagraph.h"

#include "signalaccess.h"
#include "baseutils.h"

#include <QPainter>
#include <QMouseEvent>
#include <QTime>
#include <QDebug>


AreaGraph::AreaGraph(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AreaGraph)
{

    ui->setupUi(this);

}

AreaGraph::~AreaGraph()
{
    delete ui;
}


void AreaGraph::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    //! Собственно, определяем границы и канву
    QRect geo = QRect(0, 0, geometry().width(), geometry().height());
    QPainter painter(this);

    //! И рисуем
    setCanvas(&painter, geo, this);
    doPaint(4);
}

void AreaGraph::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    onMousePressEvent(event);
    emit press(event->x(), event->y(), event->buttons());
}

void AreaGraph::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    onMouseReleaseEvent(event);
    emit release(event->x(), event->y(), event->buttons());
}

void AreaGraph::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
    onMouseMoveEvent(event);
    emit move(event->x(), event->y(), event->buttons());
}

void AreaGraph::doUpdate()
{
    update();
}

void AreaGraph::doMoveCursor()
{
    emit moveCursor();
}

