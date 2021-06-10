#ifndef CROSSRESULTDATA_H
#define CROSSRESULTDATA_H

#include <QObject>

#include "channeljsondata.h"
#include "factorsdefines.h"

/*!
 * \brief Класс для записи в БД данных о результатах теста на устойчивость CrossResultData class
 */
class CrossResultData : public ChannelJsonData
{
public:
    explicit CrossResultData(const QString &chanId);

    /*!
     * \brief Создает на основе массива байт
     * \param data - сигнал в виде массива байт
     */
    explicit CrossResultData(const QByteArray &data);

    int stageTime() const;
    void setStageTime(const int time);

    int freq() const;
    void setFreq(const int freq);

    int diap() const;
    void setDiap(const int diap);

    int eventsCount() const;
    bool event(const int idx, int& kind, int& direction, int& counter);

    void addEvent(const int kind, const int direction, const int counter);
};

/*

  Результаты теста на устойчивость хранятся в виде JsonArray
  {
      "stage_time": 15,
      "freq": 50,
      "events": [
          {
              "event_kind": 1,        // 1 - начало движения; 0 - конец движения
              "direction": 0,         // 0 - up, 1 - right, 2 - down, 3 - left
              "counter": 155
          },
          ....
      ]
  }


 */


#endif // CROSSRESULTDATA_H
