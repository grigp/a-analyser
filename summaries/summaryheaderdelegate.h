#ifndef SUMMARYHEADERDELEGATE_H
#define SUMMARYHEADERDELEGATE_H

#include <QObject>
#include <QPainter>

#include "qstyleditemdelegate.h"

class SummaryHeaderDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit SummaryHeaderDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
};

#endif // SUMMARYHEADERDELEGATE_H
