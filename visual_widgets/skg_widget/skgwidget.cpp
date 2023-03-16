#include "skgwidget.h"
#include "ui_skgwidget.h"

#include <QPainter>
#include <QDebug>



SKGWidget::SKGWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SKGWidget),
  SKGPainter ()
{
    ui->setupUi(this);
}

SKGWidget::~SKGWidget()
{
    delete ui;
}

void SKGWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QRect geo = QRect(0, 0, geometry().width(), geometry().height());

    QPainter painter(this);
//    painter.setBrush(QBrush(Qt::red));
//    painter.drawRect(geo);
    setCanvas(&painter, geo);
    doPaint(2);
}

void SKGWidget::doUpdate()
{
    update();
}
