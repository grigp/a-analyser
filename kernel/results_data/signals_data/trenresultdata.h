#ifndef TRENRESULTDATA_H
#define TRENRESULTDATA_H

#include <QObject>

#include "channeljsondata.h"
#include "factorsdefines.h"



/*!
 * \brief Класс для записи в БД данных о результатах сеанса тренинга TrenResultData class
 */
class TrenResultData : public ChannelJsonData
{
public:
    explicit TrenResultData(const QString &chanId);

    /*!
     * \brief Создает на основе массива байт
     * \param data - сигнал в виде массива байт
     */
    explicit TrenResultData(const QByteArray &data);

    /*!
     * \brief Возвращает кол-во показателей в канале
     */
    int size() const;

    /*!
     * \brief Возвращает данные о показателе из канала по номеру показателя
     */
    FactorsDefines::FactorValue factor(const int idx) const;

    /*!
     * \brief Добавляет показатель в канал (в пробах)
     * \param uid - uid показателя
     * \param value - значение показателя
     */
    void addFactor(const QString &uid, const double value);
};

/*

  Показатели тренажеров хранятся в виде JsonArray
  uid показателей соответствуют uid в группе показателей результатов сеанса тренинга
  {
      "factors": [
          {
              "uid": "{1fc8cb00-550e-4df2-b6a3-7758b3867278}",
              "value": 2.123
          },
          ....
      ]
  }


 */

#endif // TRENRESULTDATA_H
