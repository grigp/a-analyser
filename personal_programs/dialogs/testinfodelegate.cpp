#include "testinfodelegate.h"

#include <QPainter>
#include <QDebug>

#include "personalprogramdefines.h"


TestInfoDelegate::TestInfoDelegate(QObject *parent)
    : QStyledItemDelegate (parent)
{

}

void TestInfoDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto uidMethod = index.data(PersonalProgramDefines::PersonalProgram::MethodUidRole).toString();

    if (uidMethod != "")
    {
        painter->save();

        QColor colBackground = QColor(250, 250, 250);
        QColor colText = Qt::gray;

        painter->setBrush(QBrush(colBackground));
    //    painter->setPen(colBackground);
        painter->setPen(colText);
        auto rn = option.rect;
        rn.setHeight(rn.height() - 2);
        rn.setTop(rn.top() + 1);
        rn.setWidth(rn.width() - 2);
        rn.setLeft(rn.left() + 1);
        painter->drawRoundRect(rn, 15, 25);
        painter->setPen(colText);
    //    painter->drawText(rn, Qt::AlignCenter, index.data().toString() + "\n" + name + "\n" + dt);

        painter->restore();
    }
}
