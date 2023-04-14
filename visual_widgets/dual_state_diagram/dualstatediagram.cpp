#include "dualstatediagram.h"
#include "ui_dualstatediagram.h"

#include <QPainter>
#include <QDebug>

DualStateDiagram::DualStateDiagram(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DualStateDiagram)
{
    ui->setupUi(this);
}

DualStateDiagram::~DualStateDiagram()
{
    delete ui;
}

void DualStateDiagram::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    //! Собственно, определяем границы и канву
    QRect geo = QRect(0, 0, geometry().width(), geometry().height());
    QPainter painter(this);

    //! И рисуем
    setCanvas(&painter, geo);
    doPaint(1);
}

void DualStateDiagram::doUpdate()
{
    update();
}
