#ifndef ROMBERGNORMVALUEDELEGATE_H
#define ROMBERGNORMVALUEDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

class RombergNormValueDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit RombergNormValueDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const override;

};

#endif // ROMBERGNORMVALUEDELEGATE_H
