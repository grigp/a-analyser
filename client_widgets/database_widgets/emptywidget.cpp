#include "emptywidget.h"
#include "ui_emptywidget.h"

#include <QPainter>
#include <QDebug>

EmptyWidget::EmptyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EmptyWidget)
{
    ui->setupUi(this);
    m_picture = QPixmap(":/images/WallPaper.png");
}

EmptyWidget::~EmptyWidget()
{
    delete ui;
}

void EmptyWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    int x = 0;
    while (x < geometry().width())
    {
        int y = 0;
        while(y < geometry().height())
        {
            painter.drawPixmap(x, y, m_picture);
            y += m_picture.height();
        }
        x += m_picture.width();
    }

    QImage logo(":/images/Logo_BW.png");
    if (geometry().width() > logo.width())
        painter.drawImage(geometry().width() / 2 - logo.width() / 2, geometry().height() / 2 - logo.height() / 2, logo);
    else
    {
        int h = logo.height() * geometry().width() / logo.width();
        QRect r(0, geometry().height() / 2 - h / 2, geometry().width(), h);
        painter.drawImage(r, logo);
    }
}
