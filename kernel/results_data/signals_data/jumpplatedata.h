#ifndef JUMPPLATEDATA_H
#define JUMPPLATEDATA_H

#include <QObject>
#include <QJsonObject>
#include <QByteArray>

#include "channeljsondata.h"

/*!
 * \brief Класс для записи в БД данные о высоте прыжка JumpHeightData class
 */
class JumpHeightData : public ChannelJsonData
{
public:
    JumpHeightData(const QString &chanId);
    /*!
     * \brief Создает на основе массива байт
     * \param data - сигнал в виде массива байт
     */
    explicit JumpHeightData(const QByteArray &data);

    double height() const;
    void setHeight(const double height);
};


/*!
 * \brief Класс для записи в БД данные о соскакивании на платформу HoppingData class
 */
class HoppingData : public ChannelJsonData
{
public:
    HoppingData(const QString &chanId);
    /*!
     * \brief Создает на основе массива байт
     * \param data - сигнал в виде массива байт
     */
    explicit HoppingData(const QByteArray &data);

    double time() const;
    void setTime(const double time);

    double height() const;
    void setHeight(const double height);

};

#endif // JUMPPLATEDATA_H
