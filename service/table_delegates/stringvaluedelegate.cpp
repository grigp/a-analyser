#include "stringvaluedelegate.h"

#include <QPainter>
#include <QLineEdit>

StringValueDelegate::StringValueDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

void StringValueDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);
    if (option.state & QStyle::State_HasFocus)
    {
        painter->save();
        painter->setPen(QPen(QColor(0, 200, 200), 2, Qt::SolidLine));
        QRect rect = option.rect;
        rect.setTop(rect.top() + 1);
        rect.setLeft(rect.left() + 1);
        rect.setHeight(rect.height() - 1);
        rect.setWidth(rect.width() - 1);
        painter->drawRect(rect);
        painter->restore();
    }
}

QWidget *StringValueDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    QLineEdit *editor = new QLineEdit(parent);
    return editor;
}

void StringValueDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    QLineEdit *edit = static_cast<QLineEdit*>(editor);
    edit->setText(value);
}

void StringValueDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *edit = static_cast<QLineEdit*>(editor);
    QString value = edit->text();
    model->setData(index, value, Qt::EditRole);
}

void StringValueDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
