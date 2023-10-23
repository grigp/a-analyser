#ifndef TESTCOMPLETEDDELEGATE_H
#define TESTCOMPLETEDDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QPainter>

/*!
 * \brief Делегат для отображения тестов в списке дневной программв с выделением пройденных тестов The TestCompletedDelegate class
 */
class TestCompletedDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TestCompletedDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // TESTCOMPLETEDDELEGATE_H
