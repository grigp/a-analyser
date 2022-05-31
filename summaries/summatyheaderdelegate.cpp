#include "summatyheaderdelegate.h"

SummatyHeaderDelegate::SummatyHeaderDelegate(QObject *parent)
    : QStyledItemDelegate (parent)

{

}

void SummatyHeaderDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

//    painter->setBrush(QBrush(Qt::darkGray));
//    painter->setPen(Qt::darkGray);
//    painter->drawRect(option.rect);

    QLinearGradient linearGrad(option.rect.topLeft(), option.rect.bottomLeft());
    linearGrad.setColorAt(0, QColor(80, 80, 80));
    linearGrad.setColorAt(0.6, QColor(60, 60, 60));
    linearGrad.setColorAt(1, QColor(80, 80, 80));
    QBrush gradBrush(linearGrad);
    painter->setBrush(gradBrush);
    painter->setPen(Qt::darkGray);
    painter->drawRect(option.rect);

    auto txt = index.data().toString();
    painter->setPen(QPen(QColor(200, 200, 200), 1, Qt::SolidLine));
    painter->setFont(QFont("Sans", 10, QFont::Bold, false));
    auto rn = option.rect;
    rn.setWidth(rn.width() - 10);
    rn.setX(rn.x() + 5);

    int flag = Qt::AlignLeft | Qt::AlignVCenter;
    if (index.row() == 3)
        flag = Qt::AlignHCenter | Qt::AlignVCenter;
    painter->drawText(rn, flag, txt);

    painter->restore();
}
