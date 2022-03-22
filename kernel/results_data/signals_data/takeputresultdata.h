#ifndef TAKEPUTRESULTDATA_H
#define TAKEPUTRESULTDATA_H

#include <QObject>

#include "channeljsondata.h"

/*!
 * \brief Класс для записи в БД данных о процессе захвата - укладки фигур в тренажерах The TakePutResultData class
 */
class TakePutResultData : public ChannelJsonData
{
public:
    explicit TakePutResultData(const QString &chanId);
    /*!
     * \brief Создает на основе массива байт
     * \param data - сигнал в виде массива байт
     */
    explicit TakePutResultData(const QByteArray &data);

    /*!
     * \brief Тип события The Kind enum
     */
    enum Kind
    {
          tpkNone = -1
        , tpkTake = 0   ///< Захват
        , tpkPut        ///< Укладка
        , tpkError      ///< Ошибка
    };

    /*!
     * \brief Структура данных о событии The Event struct
     */
    struct Event
    {
        int counter;  ///< Счетчик пакетов при наступлении
        Kind kind;    ///< Тип события
        Event(int cnt, Kind knd)
            : counter(cnt), kind(knd) {}
    };

    /*!
     * \brief Возвращает кол-во событий в канале
     */
    int eventCount() const;

    /*!
     * \brief Возвращает данные о событии из канала по номеру события
     */
    Event event(const int idx) const;

    /*!
     * \brief Добавляет событие в канал (в пробах)
     * \param counter - значение счетчика пакетов
     * \param kind - тип события
     */
    void addEvent(const int counter, const Kind kind);

    bool isEnabledTake() const;
    void setIsEnabledTake(const bool isEnabled);

    bool isEnabledPut() const;
    void setIsEnabledPut(const bool isEnabled);

    bool isEnabledErrors() const;
    void setIsEnabledErrors(const bool isEnabled);
};

/*

  Показатели тренажеров хранятся в виде JsonArray
  uid показателей соответствуют uid в группе показателей результатов сеанса тренинга
  {
      "events": [
          {
              "counter": 201,
              "kind": 0    // 0 - take, 1 - put, 2 - error
          },
          ....
      ],
      "permissions":
      {
          "take": true,
          "put": true,
          "errors": true
      }


  }


 */

#endif // TAKEPUTRESULTDATA_H
