#include "krgwidget.h"
#include "ui_krgwidget.h"

#include <QPainter>

KRGWidget::KRGWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KRGWidget)
{
    ui->setupUi(this);
}

KRGWidget::~KRGWidget()
{
    delete ui;
}

void KRGWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    //! Собственно, определяем границы и канву
    QRect geo = QRect(0, 0, geometry().width(), geometry().height());
    QPainter painter(this);

    //! И рисуем
    setCanvas(&painter, geo, this);
    doPaint(4);
}

void KRGWidget::doUpdate()
{
    update();
}
