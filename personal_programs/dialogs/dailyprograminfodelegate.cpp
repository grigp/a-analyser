#include "dailyprograminfodelegate.h"

#include <QPainter>
#include <QDebug>

#include "personalprogramdefines.h"

DailyProgramInfoDelegate::DailyProgramInfoDelegate(QObject *parent)
    : QStyledItemDelegate (parent)
{

}

void DailyProgramInfoDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    auto name = index.data(PersonalProgramDefines::PersonalProgram::DPNameRole).toString();
    auto dt = index.data(PersonalProgramDefines::PersonalProgram::DPDateTimeRole).toString();
    if (index.row() == 0)
        dt = "23.08.2023 15:29";

    painter->save();

    QColor colBackground = QColor(230, 230, 230);
    QColor colText = Qt::black;
    if (dt != "")
    {
        colBackground = QColor(255, 255, 220);
        colText = QColor(70, 70, 0);
    }

    QRadialGradient radialGrad(QPointF(100, 100), 100);
    radialGrad.setColorAt(0, Qt::red);
    radialGrad.setColorAt(0.5, Qt::blue);
    radialGrad.setColorAt(1, Qt::green);
    painter->setBrush(radialGrad);
//    painter->setBrush(QBrush(colBackground));
//    painter->setPen(colBackground);
    painter->setPen(colText);
    auto rn = option.rect;
    rn.setHeight(rn.height() - 2);
    rn.setTop(rn.top() + 1);
    rn.setWidth(rn.width() - 2);
    rn.setLeft(rn.left() + 1);
    painter->drawRoundRect(rn, 15, 25);
    painter->setPen(colText);
    painter->drawText(rn, Qt::AlignCenter, index.data().toString() + "\n" + name + "\n" + dt);

    painter->restore();

//    if (index.row() <= 1 && nv != DataDefines::nsvMissing)
//    {
//        QColor bColor = Qt::gray;
//        QColor fColor = Qt::darkGray;
//        if (nv == DataDefines::nsvNorm)
//        {
//            bColor = QColor(150, 255, 150); // Qt::green;
//            fColor = QColor(0, 100, 50); // Qt::darkGreen;
//        }
//        else
//        if (nv == DataDefines::nsvBelow)
//        {
//            bColor = QColor(255, 255, 150); //Qt::yellow;
//            fColor = QColor(100, 100, 0); // Qt::darkYellow;
//        }
//        else
//        if (nv == DataDefines::nsvAbove)
//        {
//            bColor = QColor(255, 100, 150); //Qt::red;
//            fColor = QColor(100, 0, 0); //Qt::darkRed;
//        }

//        painter->setBrush(QBrush(bColor));
//        painter->setPen(fColor);
//        auto rn = option.rect;
//        painter->drawRect(rn);
//        painter->drawText(rn, Qt::AlignLeft, "  " + index.data().toString());
//    }
//    else
//        painter->drawText(rv, Qt::AlignLeft, index.data().toString());

//    painter->restore();
}
