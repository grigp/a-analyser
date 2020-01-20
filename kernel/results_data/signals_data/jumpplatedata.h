#ifndef JUMPPLATEDATA_H
#define JUMPPLATEDATA_H

#include <QObject>
#include <QJsonObject>
#include <QByteArray>

#include "channeljsondata.h"
#include "signalsdefines.h"
#include "baseutils.h"

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
                "height": 0.0021,      // Высота прыжка в сантиметрах
                "time_contact": 0.123  // Время контакта с платформой (сек)
            },
            ...
        ]
    }

 */
};


/*!
 * \brief Класс для записи в БД данных о результатах теппинг теста TeppingTestData class
 */
class TeppingTestData : public ChannelJsonData
{
public:
    TeppingTestData(const QString &chanId);

    /*!
     * \brief Создает на основе массива байт
     * \param data - сигнал в виде массива байт
     */
    explicit TeppingTestData(const QByteArray &data);

    double time() const;
    void setTime(const double time);

    int stepsCount(const BaseUtils::Side side) const;
    SignalsDefines::TeppingStepRec step(const BaseUtils::Side side, const int idx) const;

    void addStep(const BaseUtils::Side side, const double timeContact);

/*
    Формат канала в json:

    {
        "time_test": 100.23, // Время теста в секундах. Если задано кол-во шагов, время может быть не целым
        "left_leg": [           // Массив шагов для левой ноги
            {
                "time_contact": 0.123  // Время контакта с платформой (сек)
            },
            ...
        ],
        "right_leg": [          // Массив шагов для правой ноги
            {
                "time_contact": 0.132  // Время контакта с платформой (сек)
            },
            ...
        ]
    }
    КОЛ-ВО ШАГОВ ДЛЯ НОГ МОЖЕТ БЫТЬ РАЗНЫМ !!!

 */
};


/*!
 * \brief Класс для записи в БД данных о результатах дроп теста DropTestData class
 */
class DropTestData : public ChannelJsonData
{
public:
    DropTestData(const QString &chanId);

    /*!
     * \brief Создает на основе массива байт
     * \param data - сигнал в виде массива байт
     */
    explicit DropTestData(const QByteArray &data);

    double time() const;
    void setTime(const double time);

    int jumpsCount() const;
    SignalsDefines::DropJumpRec jump(const int idx) const;

    void addJump(double fallHeight, double height, double timeNoContact, double timeContact);

/*
    Формат канала в json:

    {
        "time_test": 100.23, // Время теста в секундах. Если задано кол-во прыжков, время может быть не целым
        "jumps": [           // Массив прыжков
            {
                "fall": 20,              // Высота степ-платформы в сантиметрах
                "height": 2.1,           // Высота прыжка в сантиметрах
                "time_contact": 0.123,   // Время контакта с платформой (сек)
                "time_no_contact":0.232  // Время бесконтактной фазы (сек)
            },
            ...
        ]
    }

 */
};


#endif // JUMPPLATEDATA_H
