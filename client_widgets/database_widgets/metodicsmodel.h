#ifndef METODICSMODEL_H
#define METODICSMODEL_H

#include <QObject>
#include <QStandardItemModel>

/*!
 * \brief Класс модели списка методик MetodicsModel class
 */
class MetodicsModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit MetodicsModel(QObject *parent = nullptr);

    /*!
     * \brief Редактировать параметры методики
     * \param rowNum - номер строки в списке методик
     */
    void editMetodicParams(QWidget *parent, const int rowNum);

    void load();
};


/*!
 * \brief Класс модели списка типов методик MetodicsKindModel class
 */
class MetodicsKindModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit MetodicsKindModel(QObject *parent = nullptr);

    ///< Номера колонок
    enum Columns
    {
          ColName = 0
    };

    enum MetodicsModelRoles
    {
        MetodicKindUidRole = Qt::UserRole + 1  ///< uid методики. QString в итеме названия ColName
    };

    void load();
};


#endif // METODICSMODEL_H
