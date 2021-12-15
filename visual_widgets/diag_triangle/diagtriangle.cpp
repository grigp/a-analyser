#include "diagtriangle.h"
#include "ui_diagtriangle.h"

#include <QPainter>

DiagTriangle::DiagTriangle(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DiagTriangle)
{
    ui->setupUi(this);
}

DiagTriangle::~DiagTriangle()
{
    delete ui;
}

void DiagTriangle::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    auto backColor = palette().background().color();
    painter.setBrush(QBrush(backColor , Qt::SolidPattern));
    painter.setPen(QPen(backColor , 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawRect(geometry());

    int mx = geometry().width() / 2;
    int my = geometry().height() / 2;

    auto font = painter.font();
    font.setPixelSize(22);
    font.setBold(true);
    painter.setFont(font);

    if (m_diap > 0)
    {
        painter.setBrush(QBrush(m_diagColor, Qt::SolidPattern));
        painter.setPen(QPen(m_diagColor, 1, Qt::SolidLine, Qt::FlatCap));
        int h = (my - 10) * m_valueUp / m_diap;
        painter.drawRect(mx - 10, my - 10 - h, 20, h);
        painter.drawText(mx + 15, 22, QString::number(m_valueUp));

        painter.translate(mx, my);
        painter.rotate(120);
        h = (my - 10) * m_valueRightDown / m_diap;
        painter.drawRect(-10, -5 - h, 20, h);
        painter.rotate(-240);
        h = (my - 10) * m_valueLeftDown / m_diap;
        painter.drawRect(-10, -5 - h, 20, h);
        painter.rotate(120);
        painter.translate(-mx, -my);

        painter.drawText(static_cast<int>(mx - (my - 10) * cos(M_PI / 6)) - 20,
                         static_cast<int>(my + (my - 10) * sin(M_PI / 6)) - 20,
                         QString::number(m_valueLeftDown));
        painter.drawText(static_cast<int>(mx + (my - 10) * cos(M_PI / 6)),
                         static_cast<int>(my + (my - 10) * sin(M_PI / 6)) - 20,
                         QString::number(m_valueRightDown));
    }

    painter.setPen(QPen(m_frameColor, 1, Qt::SolidLine, Qt::FlatCap));
    painter.setBrush(QBrush(m_frameColor, Qt::NoBrush));
    painter.drawRect(mx - 10, 0, 20, my - 10);

    painter.translate(mx, my);
    painter.rotate(60);
    painter.drawRect(-10, 5, 20, my - 10);
    painter.rotate(-120);
    painter.drawRect(-10, 5, 20, my - 10);
    painter.rotate(60);
    painter.translate(-mx, -my);

}
