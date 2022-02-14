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
          CmdAdd = 0
        , CmdUp
        , CmdDown
        , CmdDelete
    };


    explicit EditCommandDelegate(const EdditingCommand cmd,
                                 const int column,
                                 QObject *parent = nullptr);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

private:
    EdditingCommand m_command;      ///< Команда
    int m_column;           ///< Колонка
    mutable QModelIndex m_pressedIndex;
    bool m_commandsButtonVisible {true};
};

#endif // EDITCOMMANDDELEGATE_H
