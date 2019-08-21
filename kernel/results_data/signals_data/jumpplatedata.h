#ifndef JUMPPLATEDATA_H
#define JUMPPLATEDATA_H

#include <QObject>
#include <QJsonObject>

#include "channeljsondata.h"

/*!
 * \brief Класс для записи в БД данные о высоте прыжка JumpHeightData class
 */
class JumpHeightData : public ChannelJsonData
{
public:
    JumpHeightData(const QString &chanId);

    /*!
     * \brief Виртуальная функция, возвращающая идентификатор канала
     */
    QString channelId() const override;

    double height() const;
    void setHeight(const double height);

private:
    QString m_channelId;
};


/*!
 * \brief Класс для записи в БД данные о соскакивании на платформу HoppingData class
 */
class HoppingData : public ChannelJsonData
{
public:
    HoppingData(const QString &chanId);

    /*!
     * \brief Виртуальная функция, возвращающая идентификатор канала
     */
    QString channelId() const override;

    double time() const;
    void setTime(const double time);

    double height() const;
    void setHeight(const double height);

private:
    QString m_channelId;
};

#endif // JUMPPLATEDATA_H
