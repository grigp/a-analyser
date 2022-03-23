#ifndef DYNAMICDATAMODEL_H
#define DYNAMICDATAMODEL_H

#include <QObject>
#include <QStandardItemModel>

#include "factorsdefines.h"

/*!
 * \brief Модель данных для построения динамики показателей DynamicDataModel class
 * Строки - показатели
 * Столбцы - тесты, включенные в динамику
 */
class DynamicDataModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit DynamicDataModel(QObject *parent = nullptr);

    ///< Колонки
    enum Columns
    {
          ColFactor = 0      ///< Показатели
        , ColTestsFirst = 1  ///< Первая колонка тестов
    };

    ///< Роли для доступа к данным
    enum DynamicDataRoles
    {
          FactorUidRole = Qt::UserRole + 1  ///< Роль для доступа к uid показателя. QString. Колонка ColFactor.
        , DescriptionRole                   ///< Роль для доступа к описанию показателя. QString. Колонка ColFactor.
        , ProbeNumRole                      ///< Роль для доступа к номеру пробы. int. Колонка ColFactor.
        , ChannelIdRole                     ///< Роль для доступа к идентификатору канала. QString. Колонка ColFactor.
        , DateTimeRole                      ///< Роль для доступа к дате и времени обследования. QDateTime. Итемы заголовков. row() == 0.
        , TestUidRole                       ///< Роль для доступа к uid обследований. QString. Итемы заголовков. row() == 0.
        , ValueRole                         ///< Роль для доступа к значению показателя. double. Колонки, начиная с ColTestsFirst.
    };

    /*!
     * \brief Добавляет колонку с показателями для теста
     * \param factors - колонка с показателями
     * \param testUid - uid теста
     * \param dt - дата и время теста
     */
    void addTestData(const QList<FactorsDefines::FactorValueAdvanced> &factors,
                     const QString &testUid, const QDateTime &dt);

    void clear();

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

private:
    int getColumnNumberByDateTimeTest(const QDateTime &dt) const;

    int m_testCounter {0};
};

#endif // DYNAMICDATAMODEL_H
