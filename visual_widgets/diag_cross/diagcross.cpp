#include "diagcross.h"
#include "ui_diagcross.h"

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
    Q_UNUSED(event);

    QPainter painter(this);

    painter.setBrush(QBrush(m_backgroundColor, Qt::SolidPattern));
    painter.setPen(QPen(m_backgroundColor, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawRect(geometry());

    int mx = geometry().width() / 2;
    int my = geometry().height() / 2;

    if (m_diap > 0)
    {
        painter.setBrush(QBrush(m_frontalColor, Qt::SolidPattern));
        painter.setPen(QPen(m_frontalColor, 1, Qt::SolidLine, Qt::FlatCap));
        int w = (mx - 10) * m_valueLeft / m_diap;
        painter.drawRect(mx - 10 - w, my - 10, w, 20);
        w = (mx - 10) * m_valueRight / m_diap;
        painter.drawRect(mx + 10, my - 10, w, 20);

        painter.setBrush(QBrush(m_sagittalColor, Qt::SolidPattern));
        painter.setPen(QPen(m_sagittalColor, 1, Qt::SolidLine, Qt::FlatCap));
        int h = (my - 10) * m_valueUp / m_diap;
        painter.drawRect(mx - 10, my - 10 - h, 20, h);
        h = (my - 10) * m_valueDown / m_diap;
        painter.drawRect(mx - 10, my + 10, 20, h);
    }
}
