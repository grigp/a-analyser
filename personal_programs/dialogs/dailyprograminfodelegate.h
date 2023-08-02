#ifndef DAILYPROGRAMINFODELEGATE_H
#define DAILYPROGRAMINFODELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

/*!
 * \brief Делегат отображения данных о дневной программе в таблице динамики индивидуальной программы The DailyProgramInfoDelegate class
 */
class DailyProgramInfoDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit DailyProgramInfoDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // DAILYPROGRAMINFODELEGATE_H
