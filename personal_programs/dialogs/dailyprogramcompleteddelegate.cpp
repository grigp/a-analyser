#include "dailyprogramcompleteddelegate.h"

#include <QPainter>
#include <QTableView>

#include "activepersonalprogrameditor.h"

DailyProgramCompletedDelegate::DailyProgramCompletedDelegate(QObject *parent)
    : QStyledItemDelegate (parent)
{

}

void DailyProgramCompletedDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto isCompleted = index.data(ActivePersonalProgramEditor::DPCompletedRole).toBool();

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
    if (isCompleted)
    {
        colText = Qt::lightGray;
    }

    painter->setPen(colText);
    painter->drawText(rn, Qt::AlignLeft | Qt::AlignVCenter, index.data().toString());

    painter->restore();
}
