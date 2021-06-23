#ifndef STEPOFFSETRESULTDATA_H
#define STEPOFFSETRESULTDATA_H

#include <QObject>

#include "channeljsondata.h"
#include "factorsdefines.h"

class StepOffsetResultData : public ChannelJsonData
{
public:
    explicit StepOffsetResultData(const QString &chanId);
    /*!
     * \brief Создает на основе массива байт
     * \param data - сигнал в виде массива байт
     */
    explicit StepOffsetResultData(const QByteArray &data);

    int stageTime() const;
    void setStageTime(const int time);

    int freq() const;
    void setFreq(const int freq);

    int diap() const;
    void setDiap(const int diap);

    int force() const;
    void setForce(const int force);

    int direction() const;
    void setDirection(const int dir);

    int stepsCount() const;
    bool step(const int idx, int& counterTo, int& counterFrom);

    void addStep(const int counterTo, const int counterFrom);
};

/*

  Результаты теста "Ступени" хранятся в виде JsonArray
  {
      "stage_time": 8,
      "freq": 50,
      "diap": 128,
      "force": 45,
      "steps": [
          {
              "counter_to": 400,    // Момент начала ступени в отсчетах
              "counter_from": 800   // Момент завершения ступени в отсчетах
          },
          ....
      ]
  }


 */


#endif // STEPOFFSETRESULTDATA_H
