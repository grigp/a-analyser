#include "skgwidget.h"
#include "ui_skgwidget.h"

#include <QPainter>
#include <QDebug>

SKGWidget::SKGWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SKGWidget)
{
    ui->setupUi(this);
}

SKGWidget::~SKGWidget()
{
    delete ui;
}

void SKGWidget::paintEvent(QPaintEvent *event)
{
//    QWidget::paintEvent(event);

    qDebug() << geometry();
    QPainter painter(this);
    painter.setBrush(QBrush(Qt::red));
    painter.drawRect(geometry());
    setCanvas(&painter, geometry());
    doPaint(1);
}
