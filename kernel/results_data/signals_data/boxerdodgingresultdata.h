#ifndef BOXERDODGINGRESULTDATA_H
#define BOXERDODGINGRESULTDATA_H

#include <QObject>

#include "channeljsondata.h"

class BoxerDodgingResultData : public ChannelJsonData
{
public:
    explicit BoxerDodgingResultData(const QString &chanId);

    /*!
     * \brief Создает на основе массива байт
     * \param data - сигнал в виде массива байт
     */
    explicit BoxerDodgingResultData(const QByteArray &data);

    int deviationThreshold() const;
    void setDeviationThreshold(const int value);

    int freq() const;
    void setFreq(const int freq);

    int diap() const;
    void setDiap(const int diap);

    int stagesCount() const;
    bool stage(const int idx, int& code, int& counter);

    void addStage(const int code, const int counter);
};



/*

  Результаты тренажера с уклонением хранятся в виде JsonArray
  {
      "freq": 50,
      "diap": 128,
      "deviation_threshold": 30,
      "stages": [
          {
              "code": 1,              // код цели: 0 - базовая, 1 - левое уклонение, 2 - правое уклонение, 3 - подстановка вперед, 4 - подстановка назад
              "counter": 100          // Номер отсчета начала этапа от начала сигнала
          },
          ....
      ]
  }


 */

#endif // BOXERDODGINGRESULTDATA_H
