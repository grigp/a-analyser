#include "dailyprograminfodelegate.h"

#include <QPainter>
#include <QDebug>

#include "personalprogramdefines.h"
#include "personalprogrammanager.h"

DailyProgramInfoDelegate::DailyProgramInfoDelegate(QObject *parent)
    : QStyledItemDelegate (parent)
{

}

void DailyProgramInfoDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    auto name = index.data(PersonalProgramDefines::PersonalProgram::DPNameRole).toString();
    auto dt = index.data(PersonalProgramDefines::PersonalProgram::DPDateTimeRole).toString();
//    if (index.row() == 0)         //TODO: Убрать!!!
//        dt = "23.08.2023 15:29";

    painter->save();

    QColor colBackground = QColor(200, 200, 200);
    QColor colText = Qt::black;
    if (dt != "")
    {
        colBackground = QColor(230, 230, 130);
        colText = QColor(70, 70, 0);
    }

    QColor colBackgroundLt = PersonalProgramManager::lightColor(colBackground, 2);

    auto rn = option.rect;

    QLinearGradient gradient(QPointF(rn.left() + rn.width() / 2, rn.top()), QPointF(rn.left() + rn.width() / 2, rn.top() + rn.height()));
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(0.2, colBackgroundLt);
    gradient.setColorAt(0.5, colBackground);
    gradient.setColorAt(0.8, colBackgroundLt);
    gradient.setColorAt(1, Qt::white);
    painter->setBrush(gradient);
    painter->setPen(colText);
    rn.setHeight(rn.height() - 2);
    rn.setTop(rn.top() + 1);
    rn.setWidth(rn.width() - 2);
    rn.setLeft(rn.left() + 1);
    painter->drawRoundRect(rn, 15, 25);
    painter->setPen(colText);
    painter->drawText(rn, Qt::AlignCenter, index.data().toString() + "\n" + name + "\n" + dt);

    painter->restore();

}
