#ifndef STRINGVALUEDELEGATE_H
#define STRINGVALUEDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

/*!
 * \brief Класс делегата редактирования строковых значений StringValueDelegate class
 */
class StringValueDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit StringValueDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
};

#endif // STRINGVALUEDELEGATE_H
