#include "comboboxdelegate.h"

#include <QPainter>
#include <QComboBox>

ComboBoxDelegate::ComboBoxDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void ComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
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

QWidget *ComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    QComboBox *editor = new QComboBox(parent);

    for (int i = 0; i < m_values.size(); ++i)
    {
        editor->addItem(m_values.at(i).first);
        editor->setItemData(i, m_values.at(i).second, ValueIdRole);
    }

    return editor;
}

void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    QComboBox *cb = static_cast<QComboBox*>(editor);
    cb->setCurrentText(value);
}

void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *edit = static_cast<QComboBox*>(editor);
    model->setData(index, edit->currentText(), Qt::EditRole);
}

void ComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

void ComboBoxDelegate::setValues(const QList<std::pair<QString, QVariant> > &vals)
{
    m_values = vals;
}
