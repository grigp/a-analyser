#ifndef BILATERALRESULTDATA_H
#define BILATERALRESULTDATA_H

#include <QObject>
#include <QRect>

#include "channeljsondata.h"

/*!
 * \brief Класс результатов данных билатерального режима The BilateralResultData class
 */
class BilateralResultData : public ChannelJsonData
{
public:
    BilateralResultData(const QString &chanId);

    /*!
     * \brief Создает на основе массива байт
     * \param data - сигнал в виде массива байт
     */
    explicit BilateralResultData(const QByteArray &data);

    /*!
     * \brief Возвращает кол-во платформ
     */
    int platformsCount() const;

    /*!
     * \brief Возвращает положение и размеры платформы
     * \param idx - номер платформы
     */
    QRect platform(const int idx) const;

    /*!
     * \brief Добавляет запись о платформе
     * \param platform - положение и размеры платформы
     */
    void addPlatform(const QRect &platform);
};


/*

  Результаты хранятся в виде JsonArray
  Расположение и размеры платформ
  {
      "platforms": [
          {
              "x": -500,
              "y": 250,
              "width": 500,
              "height": 500
          },
          ....
      ]
  }


 */

#endif // BILATERALRESULTDATA_H
