#ifndef OCTAEDRONRESULTDATA_H
#define OCTAEDRONRESULTDATA_H

#include <QObject>

#include "channeljsondata.h"

class OctaedronResultData : public ChannelJsonData
{
public:
    explicit OctaedronResultData(const QString &chanId);

    /*!
     * \brief Создает на основе массива байт
     * \param data - сигнал в виде массива байт
     */
    explicit OctaedronResultData(const QByteArray &data);

    int stageTime() const;
    void setStageTime(const int time);

    QString circeRoundRuleMode() const;
    void setCirceRoundRuleMode(const QString crm);

    QString directionMode() const;
    void setDirectionMode(const QString dm);

    int radius() const;
    void setRadius(const int radius);

    int freq() const;
    void setFreq(const int freq);

    int diap() const;
    void setDiap(const int diap);

    int stagesCount() const;
    bool stage(const int idx, int& code, int& counter, double& x, double& y);

    void addStage(const int code, const int counter, const double x, const double y);
};

/*

  Результаты тренажера Октаэдр хранятся в виде JsonArray
  {
      "stage_time": 5,
      "freq": 50,
      "diap": 128,
      "radius": 50,
      "circe_round_rule_mode": "radial";   // radial или circle
      "stages": [
          {
              "code": 1,              // код цели: -1 - центр, 0 -:- 7 - от верха против часовой
              "counter": 100,         // Номер отсчета начала этапа от начала сигнала
              "x": 0,                 // координаты цели в мм
              "y": 0
          },
          ....
      ]
  }


 */

#endif // OCTAEDRONRESULTDATA_H
