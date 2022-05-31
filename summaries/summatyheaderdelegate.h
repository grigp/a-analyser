#ifndef SUMMATYHEADERDELEGATE_H
#define SUMMATYHEADERDELEGATE_H

#include <QObject>
#include <QPainter>

#include "qstyleditemdelegate.h"

class SummatyHeaderDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit SummatyHeaderDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
};

#endif // SUMMATYHEADERDELEGATE_H
