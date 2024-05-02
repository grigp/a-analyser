#ifndef TESTINFODELEGATE_H
#define TESTINFODELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

/*!
 * \brief Делегат отображения данных о тесте в таблице динамики индивидуальной программы The TestInfoDelegate class
 */
class TestInfoDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TestInfoDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const override;

};

#endif // TESTINFODELEGATE_H
