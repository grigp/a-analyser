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
}
