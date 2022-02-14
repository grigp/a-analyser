#include "buttondelegate.h"

#include <QApplication>
#include <QPushButton>
#include <QDebug>

ButtonDelegate::ButtonDelegate(QObject *parent)
    : QStyledItemDelegate (parent)
{

}

void ButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
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
    opt.text = "x";
    if (index.model()->data(index, role()).toBool())
        opt.state |= QStyle::State_On;
    QApplication::style()->drawControl(QStyle::CE_PushButton, &opt, painter);
}

QWidget *ButtonDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    auto *editor = new QPushButton(parent);
    editor->setAutoDefault(true);
    editor->setText("x");
    connect(editor, &QPushButton::clicked, this, [=](){
        qDebug() << "push";
    });
    qDebug() << Q_FUNC_INFO;
    return editor;
}

void ButtonDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    Q_UNUSED(editor);
    Q_UNUSED(index);

}

void ButtonDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    Q_UNUSED(editor);
    Q_UNUSED(model);
    Q_UNUSED(index);

}

void ButtonDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
