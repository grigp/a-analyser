#ifndef DAILYPROGRAMCOMPLETEDDELEGATE_H
#define DAILYPROGRAMCOMPLETEDDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

/*!
 * \brief Делегат для отображения дневных программ в списке с выделением пройденных ДП The DaylyProgramCompletedDelegate class
 */
class DailyProgramCompletedDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit DailyProgramCompletedDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // DAILYPROGRAMCOMPLETEDDELEGATE_H
