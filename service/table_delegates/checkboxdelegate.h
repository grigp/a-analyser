#ifndef CHECKBOXDELEGATE_H
#define CHECKBOXDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QPainter>

class CheckBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CheckBoxDelegate(QObject *parent = nullptr);

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

protected:
    virtual int role() const {return Qt::UserRole;}
};

#endif // CHECKBOXDELEGATE_H
