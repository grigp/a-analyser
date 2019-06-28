#include "checkboxdelegate.h"
#include <QCheckBox>
#include <QApplication>

CheckBoxDelegate::CheckBoxDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

void CheckBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
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

    QStyleOptionButton opt;
    opt.rect = option.rect;
    opt.state |= QStyle::State_Enabled;
    opt.state |= QStyle::State_Active;
    if (index.model()->data(index, role()).toBool())
        opt.state |= QStyle::State_On;
    QApplication::style()->drawControl(QStyle::CE_CheckBox, &opt, painter);

}

QWidget *CheckBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto *editor = new QCheckBox(parent);
    editor->setText("");
    return editor;
}

void CheckBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    auto value = index.model()->data(index, role()).toBool();
    static_cast<QCheckBox*>(editor)->setChecked(value);
}

void CheckBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QCheckBox* cb = static_cast<QCheckBox*>(editor);
    model->setData(index, cb->checkState(), role());
}

void CheckBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
