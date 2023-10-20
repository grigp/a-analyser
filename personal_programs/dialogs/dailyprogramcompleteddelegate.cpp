#include "dailyprogramcompleteddelegate.h"

#include <QPainter>
#include <QTableView>

#include "personalprogramdefines.h"

DailyProgramCompletedDelegate::DailyProgramCompletedDelegate(QObject *parent)
    : QStyledItemDelegate (parent)
{

}

void DailyProgramCompletedDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto completed = index.data(PersonalProgramDefines::DPCompletedRole).toInt();

    painter->save();
    auto rn = option.rect;

    //! Выделение итема
    if (QAbstractItemView* tableView = qobject_cast<QAbstractItemView*>(this->parent()))
    {
        auto selIdxs = tableView->selectionModel()->selectedIndexes();
        if (selIdxs.contains(index))
        {
            painter->setBrush(QColor(193, 220, 247));
            painter->setPen(QColor(193, 220, 247));
            painter->drawRect(rn);
        }
    }

    QColor colText = Qt::black;
    if (completed == PersonalProgramDefines::dpcvCompleted)
        colText = Qt::lightGray;
    else
    if (completed == PersonalProgramDefines::dpcvInterrupted)
        colText = Qt::darkRed;

    painter->setPen(colText);
    painter->drawText(rn, Qt::AlignLeft | Qt::AlignVCenter, index.data().toString());

    painter->restore();
}
