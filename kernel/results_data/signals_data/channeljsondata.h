#ifndef CHANNELJSONDATA_H
#define CHANNELJSONDATA_H

#include <QObject>
#include <QJsonObject>

#include "channeldata.h"

/*!
 * \brief Класс для сохранения разнородных данных канала ChannelJsonData class
 * Сохраняет данные в виде json, серилизованного в QByteArray
 */
class ChannelJsonData : public ChannelData
{
public:
    ChannelJsonData();
    virtual ~ChannelJsonData();

    QJsonObject data() const;
    void setData(const QJsonObject data);

    /*!
     * \brief Виртуальная функция, возвращающая идентификатор канала
     */
    virtual QString channelId() const = 0;

    /*!
     * \brief Виртуальная функция, читает данные из массива байт
     * \param data - данные в виде массива байт
     */
    virtual void fromByteArray(const QByteArray &data);

    /*!
     * \brief Виртуальная функция, записывает данные в массив байт
     * \param data - массив, в которые будут записаны данные
     */
    virtual void toByteArray(QByteArray &data) const;

    /*!
     * \brief Очищает данные
     */
    void clear() override;

private:
    QJsonObject m_data = QJsonObject();
};

#endif // CHANNELJSONDATA_H
