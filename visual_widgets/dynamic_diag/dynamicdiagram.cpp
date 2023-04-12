#include "dynamicdiagram.h"
#include "ui_dynamicdiagram.h"

#include "baseutils.h"

#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

DynamicDiagram::DynamicDiagram(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DynamicDiagram)
{
    ui->setupUi(this);
}

DynamicDiagram::~DynamicDiagram()
{
    delete ui;
}

void DynamicDiagram::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    //! Собственно, определяем границы и канву
    QRect geo = QRect(0, 0, geometry().width(), geometry().height());
    QPainter painter(this);

    //! И рисуем
    setCanvas(&painter, geo);
    doPaint(1);
}

void DynamicDiagram::mousePressEvent(QMouseEvent *event)
{
    auto selItm = doSelectItemByX(event->x());
    emit selectItem(selItm);

    QWidget::mousePressEvent(event);
}

void DynamicDiagram::doUpdate()
{
    update();
}

