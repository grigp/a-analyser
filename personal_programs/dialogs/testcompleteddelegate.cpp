#include "testcompleteddelegate.h"

#include <QTableView>
#include <QStandardItemModel>
#include <QDebug>

#include "personalprogramdefines.h"

TestCompletedDelegate::TestCompletedDelegate(QObject *parent)
    : QStyledItemDelegate (parent)
{

}

void TestCompletedDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto completed = index.data(PersonalProgramDefines::PersonalProgram::TestUidRole).toString();

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
    if (completed != "")
        colText = Qt::lightGray;

    auto vIcon = index.data(Qt::DecorationRole);
    QIcon icon = vIcon.value<QIcon>();
    painter->drawPixmap(QRect(rn.x(), rn.y(), 24, 24), icon.pixmap(32, 32), QRect(0, 0, 32, 32));

    painter->setPen(colText);
    painter->drawText(QRect(rn.x() + 30, rn.y(), rn.width() - 30, rn.height()), Qt::AlignLeft | Qt::AlignVCenter, index.data().toString());

    painter->restore();
}
