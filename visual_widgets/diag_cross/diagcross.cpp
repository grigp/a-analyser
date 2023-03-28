#include "diagcross.h"
#include "ui_diagcross.h"

#include <QDebug>
#include <QPainter>

DiagCross::DiagCross(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DiagCross)
{
    ui->setupUi(this);
}

DiagCross::~DiagCross()
{
    delete ui;
}

void DiagCross::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    //! Собственно, определяем границы и канву
    QRect geo = QRect(0, 0, geometry().width(), geometry().height());
    QPainter painter(this);

    //! И рисуем
    setCanvas(&painter, geo);
    doPaint(2);
}

void DiagCross::doUpdate()
{
    update();
}
