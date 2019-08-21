#ifndef CHANNELDATA_H
#define CHANNELDATA_H

#include <QObject>

/*!
 * \brief Класс для сохранения данных канала ChannelData class
 */
class ChannelData
{
public:
    ChannelData();
    virtual ~ChannelData();

    /*!
     * \brief Виртуальная функция, возвращающая идентификатор канала
     */
    virtual QString channelId() const = 0;

    /*!
     * \brief Виртуальная функция, читает данные из массива байт
     * \param data - данные в виде массива байт
     */
    virtual void fromByteArray(const QByteArray &data) = 0;

    /*!
     * \brief Виртуальная функция, записывает данные в массив байт
     * \param data - массив, в которые будут записаны данные
     */
    virtual void toByteArray(QByteArray &data) const = 0;

    /*!
     * \brief Очищает данные
     */
    virtual void clear() = 0;
};

#endif // CHANNELDATA_H
