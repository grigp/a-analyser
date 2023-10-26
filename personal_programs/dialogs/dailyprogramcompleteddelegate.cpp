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
    //! Состояние проведенности текущей ДП
    auto completed = index.data(PersonalProgramDefines::DPCompletedRole).toInt();

    //! Состояние проведенности следующей ДП
    PersonalProgramDefines::DPCompletedValue complNext = PersonalProgramDefines::dpcvNotStarted;
    auto idxNext = index.model()->index(index.row() + 1, index.column());
    if  (idxNext.isValid())
        complNext = static_cast<PersonalProgramDefines::DPCompletedValue>(idxNext.data(PersonalProgramDefines::DPCompletedRole).toInt());
    //! Если следующая ДП была начата, то эта будет помечена, как завершенная
    if (complNext != PersonalProgramDefines::dpcvNotStarted)
        completed = PersonalProgramDefines::dpcvCompleted;

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
