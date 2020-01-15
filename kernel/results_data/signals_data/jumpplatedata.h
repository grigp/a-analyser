#ifndef JUMPPLATEDATA_H
#define JUMPPLATEDATA_H

#include <QObject>
#include <QJsonObject>
#include <QByteArray>

#include "channeljsondata.h"
#include "signalsdefines.h"

/*!
 * \brief Класс для записи в БД данных о высоте одиночного прыжка JumpHeightSingleData class
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
 * \brief Класс для записи в БД данных о соскакивании на платформу HoppingData class
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

/*!
 * \brief Класс для записи в БД данных о высоте прыжков JumpHeightData class
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

    double time() const;
    void setTime(const double time);

    int jumpsCount() const;
    SignalsDefines::JumpRec jump(const int idx) const;

    void addJump(const double height, const double timeContact);

/*
    Формат канала в json:

    {
        "time_test": 100.23, // Время теста в секундах. Если задано кол-во прыжков, время может быть не целым
        "jumps": [           // Массив прыжков
            {
                "height": 0.0021,      // Высота прыжка в метрах
                "time_contact": 0.123  // Время контакта с платформой (сек)
            },
            ...
        ]
    }

 */
};
#endif // JUMPPLATEDATA_H
