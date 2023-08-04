#include "testinfodelegate.h"

#include <QPainter>
#include <QAbstractItemView>
#include <QDebug>

#include "aanalyserapplication.h"
#include "metodicsfactory.h"
#include "personalprogramdefines.h"
#include "baseutils.h"


TestInfoDelegate::TestInfoDelegate(QObject *parent)
    : QStyledItemDelegate (parent)
{

}

void TestInfoDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto uidMethod = index.data(PersonalProgramDefines::PersonalProgram::MethodUidRole).toString();

    if (uidMethod != "")
    {
        MetodicDefines::MetodicInfo mi =
                static_cast<AAnalyserApplication*>(QApplication::instance())->
                getMetodics()->metodic(uidMethod);
        auto uidTest = index.data(PersonalProgramDefines::PersonalProgram::TestUidRole).toString();
        painter->save();

        QColor colBackground = QColor(250, 250, 250);
        QColor colText = Qt::gray;
        if (uidTest != "")
        {
            colBackground = QColor(200, 255, 200);
            colText = QColor(0, 100, 0);
        }

        painter->setBrush(QBrush(colBackground));
    //    painter->setPen(colBackground);
        painter->setPen(colText);
        auto rn = option.rect;
        rn.setHeight(rn.height() - 2);
        rn.setTop(rn.top() + 1);
        rn.setWidth(rn.width() - 2);
        rn.setLeft(rn.left() + 1);
        painter->drawRoundRect(rn, 15, 25);

        painter->drawText(rn, Qt::AlignTop | Qt::AlignHCenter, mi.name);

        //! Выделение итема
        if (QAbstractItemView* tableView = qobject_cast<QAbstractItemView*>(this->parent()))
        {
            QModelIndex hover = tableView->indexAt(tableView->viewport()->mapFromGlobal(QCursor::pos()));
            if (hover.row() == index.row() && hover.column() == index.column())
            {
                painter->setBrush(QBrush(Qt::NoBrush));
                painter->setPen(Qt::red);
                painter->drawRect(rn);
            }
        }
        painter->restore();
    }
}

