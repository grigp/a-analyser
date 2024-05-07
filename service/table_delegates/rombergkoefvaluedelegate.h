#ifndef ROMBERGKOEFVALUEDELEGATE_H
#define ROMBERGKOEFVALUEDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

/*!
 * \brief Класс делегата значений норм для коэффициентов Ромберга The RombergKoefValueDelegate class
 */
class RombergKoefValueDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit RombergKoefValueDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // ROMBERGKOEFVALUEDELEGATE_H
