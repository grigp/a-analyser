#ifndef EDITCOMMANDDELEGATE_H
#define EDITCOMMANDDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

/*!
 * \brief Делегат - командная кнопка EditCommandDelegate class
 */
class EditCommandDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:

    enum EdditingCommand
    {
        cmdNone = 0
        , CmdAdd
        , CmdUp
        , CmdDown
        , CmdDelete
        , CmdClose
        , CmdExport
    };

    enum Roles
    {
        CommandRole = Qt::UserRole + 1  ///< Роль, определяющая, какая кнопка будет отображаться
    };

    explicit EditCommandDelegate(const EdditingCommand cmd,
                                 const int column,
                                 QObject *parent = nullptr);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

private:
    QIcon getIcon(EdditingCommand cmd) const;

    EdditingCommand m_command {cmdNone};      ///< Команда
    int m_column;           ///< Колонка
    mutable QModelIndex m_pressedIndex;
    bool m_commandsButtonVisible {true};
};

#endif // EDITCOMMANDDELEGATE_H
