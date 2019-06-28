#ifndef COMBOBOXDELEGATE_H
#define COMBOBOXDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

/*!
 * \brief Класс делегата в виде выпадающего списка ComboBoxDelegate class
 * Значения добавляются с использованием функции setValues
 */
class ComboBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ComboBoxDelegate(QObject *parent = nullptr);

    enum ComboBoxDelegateRoles
    {
        ValueIdRole = Qt::UserRole + 1
    };

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;

    /*!
     * \brief Устанавливает список значений в виде списка строк и позволяет настроить делегат
     * \param vals - значения в виде списка строк
     */
    void setValues(const QList<std::pair<QString, QVariant>> &vals);

private:
    ///< Хранит список вариантов выпадающего списка
    /// Первый параметр - название, второй - пользовательские данные
    QList<std::pair<QString, QVariant>> m_values;
};

#endif // COMBOBOXDELEGATE_H
