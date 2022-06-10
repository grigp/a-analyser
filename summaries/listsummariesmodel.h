#ifndef LISTSUMMARIESMODEL_H
#define LISTSUMMARIESMODEL_H

#include <QObject>
#include <QStandardItemModel>

/*!
 * \brief Класс модели списка сводок The ListSummariesModel class
 */
class ListSummariesModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit ListSummariesModel(QObject *parent = nullptr);

    /*!
     * \brief Роли в модели списка сводок The OpenSummaryRoles enum
     */
    enum OpenSummaryRoles
    {
          FileNameRole = Qt::UserRole + 1
        , MethodicRole
        , KindRole
    };

    /*!
     * \brief Колонки в модели The Columns enum
     */
    enum Columns
    {
          colName = 0
        , colMethodic
        , colKind
    };

    static QString columnTitle(const Columns column);
};

#endif // LISTSUMMARIESMODEL_H
