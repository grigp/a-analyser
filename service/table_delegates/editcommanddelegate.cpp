#include "editcommanddelegate.h"

#include <QPainter>
#include <QApplication>

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
        switch (m_command) {
        case CmdDelete:
            icon = QIcon(":/images/delete.png");
            break;
        case CmdAdd:
            icon = QIcon(":/images/add.png");
            break;
        case CmdUp:
            icon = QIcon(":/images/up.png");
            break;
        case CmdDown:
            icon = QIcon(":/images/down.png");
            break;
        case CmdClose:
            icon = QIcon(":/images/Close.png");
            break;
        }
        icon.paint(painter, iconRect, Qt::AlignRight | Qt::AlignVCenter);
    }
}
