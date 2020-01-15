#ifndef JUMPPLATEDATA_H
#define JUMPPLATEDATA_H

#include <QObject>
#include <QJsonObject>
#include <QByteArray>

#include "channeljsondata.h"

/*!
 * \brief Класс для записи в БД данные о высоте одиночного прыжка JumpHeightSingleData class
 */
class JumpHeightSingleData : public ChannelJsonData
{
public:
    JumpHeightSingleData(const QString &chanId);
    /*!
     * \brief Создает на основе массива байт
     * \param data - сигнал в виде массива байт
     */
    explicit JumpHeightSingleData(const QByteArray &data);

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
