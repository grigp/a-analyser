#include "diagoctaedron.h"
#include "ui_diagoctaedron.h"

#include <QPainter>
#include <QDebug>


DiagOctaedron::DiagOctaedron(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DiagOctaedron)
{
    ui->setupUi(this);
}

DiagOctaedron::~DiagOctaedron()
{
    delete ui;
}

void DiagOctaedron::doUpdate()
{
    update();
}

void DiagOctaedron::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    //! Собственно, определяем границы и канву
    QRect geo = QRect(0, 0, geometry().width(), geometry().height());
    QPainter painter(this);

    //! И рисуем
    setCanvas(&painter, geo);
    doPaint(1);
}


