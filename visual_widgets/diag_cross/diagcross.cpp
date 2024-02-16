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
    int size = geometry().width();
    int l = 0;
    int t = geometry().height() / 2 - size / 2;
    if (geometry().height() < geometry().width())
    {
        size = geometry().height();
        l = geometry().width() / 2 - size / 2;
        t = 0;
    }
    QRect geo = QRect(l, t, size, size);
    QPainter painter(this);

    //! И рисуем
    setCanvas(&painter, geo);
    doPaint(2);
}

void DiagCross::doUpdate()
{
    update();
}
