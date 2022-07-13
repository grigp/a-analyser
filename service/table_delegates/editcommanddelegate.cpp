#include "editcommanddelegate.h"

#include <QPainter>
#include <QApplication>
#include <QDebug>

EditCommandDelegate::EditCommandDelegate(const EdditingCommand cmd,
                                         const int column,
                                         QObject *parent)
    : QStyledItemDelegate(parent)
    , m_command(cmd)
    , m_column(column)
{

}

void EditCommandDelegate::paint(QPainter *painter,
                                const QStyleOptionViewItem &option,
                                const QModelIndex &index) const
{
    if (option.state)// & QStyle::State_Selected)
    {
        if ((QApplication::mouseButtons() & Qt::LeftButton) == 0)
            m_pressedIndex = QModelIndex();
        QBrush brush = option.palette.alternateBase();
        if (index == m_pressedIndex)
            brush = option.palette.dark();
        painter->fillRect(option.rect, brush);
    }

    QStyledItemDelegate::paint(painter, option, index);

    QRect iconRect(option.rect.left(),
                   option.rect.top(),
                   option.rect.height(),
                   option.rect.height());

    if (m_commandsButtonVisible &&  (index.column() == m_column)
            && option.state) // & QStyle::State_Selected)
    {
        QIcon icon;
        EdditingCommand cmd = static_cast<EdditingCommand>(index.data(CommandRole).toInt());
        if (cmd != cmdNone)
            icon = getIcon(cmd);
        else
            icon = getIcon(m_command);
        icon.paint(painter, iconRect, Qt::AlignRight | Qt::AlignVCenter);
    }
}

QIcon EditCommandDelegate::getIcon(EditCommandDelegate::EdditingCommand cmd) const
{
    switch (cmd)
    {
    case CmdDelete:
        return QIcon(":/images/delete.png");
    case CmdAdd:
        return QIcon(":/images/add.png");
    case CmdUp:
        return QIcon(":/images/up.png");
    case CmdDown:
        return QIcon(":/images/down.png");
    case CmdClose:
        return QIcon(":/images/Close.png");
    case CmdExport:
        return QIcon(":/images/SignalExport1.png");
    case cmdNone:
        return QIcon();
    }
}
