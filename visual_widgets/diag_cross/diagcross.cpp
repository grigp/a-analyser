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

    auto font = painter.font();
    font.setPixelSize(22);
    font.setBold(true);
    painter.setFont(font);

    if (m_diap > 0)
    {
        painter.setBrush(QBrush(m_frontalColor, Qt::SolidPattern));
        painter.setPen(QPen(m_frontalColor, 1, Qt::SolidLine, Qt::FlatCap));
        int w = (mx - 10) * m_valueLeft / m_diap;
        painter.drawRect(mx - 10 - w, my - 10, w, 20);
        w = (mx - 10) * m_valueRight / m_diap;
        painter.drawRect(mx + 10, my - 10, w, 20);
        painter.drawText(10, my + 35, QString::number(m_valueLeft));
        painter.drawText(geometry().width() - 50, my + 35, QString::number(m_valueRight));

        painter.setBrush(QBrush(m_sagittalColor, Qt::SolidPattern));
        painter.setPen(QPen(m_sagittalColor, 1, Qt::SolidLine, Qt::FlatCap));
        int h = (my - 10) * m_valueUp / m_diap;
        painter.drawRect(mx - 10, my - 10 - h, 20, h);
        h = (my - 10) * m_valueDown / m_diap;
        painter.drawRect(mx - 10, my + 10, 20, h);
        painter.drawText(mx + 15, 22, QString::number(m_valueUp));
        painter.drawText(mx + 15, geometry().height() - 10, QString::number(m_valueDown));
    }

    painter.setPen(QPen(m_frameColor, 1, Qt::SolidLine, Qt::FlatCap));
    painter.setBrush(QBrush(m_frameColor, Qt::NoBrush));
    painter.drawRect(0, my - 10, mx - 10, 20);
    painter.drawRect(mx + 10, my - 10, mx - 10, 20);
    painter.drawRect(mx - 10, 0, 20, my - 10);
    painter.drawRect(mx - 10, my + 10, 20, my - 10);

}
