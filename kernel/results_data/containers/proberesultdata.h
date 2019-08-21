#ifndef PROBERESULTDATA_H
#define PROBERESULTDATA_H

#include <QList>

#include "channeldata.h"

/*!
 * \brief Класс, хранящий в себе результаты пробы ProbeResultData class
 * Осуществляется навигация по записанным в пробе сигналам
 */
class ProbeResultData
{
public:
    explicit ProbeResultData(const QString &testUid, const QString &uid, const QString &name);

    /*!
     * \brief Сохраняет пробу в БД
     */
    void saveProbe(const QString &testUid, const int step);

    /*!
     * \brief очищает данные пробы
     */
    void clear();

    /*!
     * \brief Возвращает uid пробы
     */
    QString uid() const;

    /*!
     * \brief Возвращает название пробы
     */
    QString name() const;

    /*!
     * \brief Возвращает кол-во сигналов в пробе
     */
    int signalsCount() const;

    /*!
     * \brief Возвращает указатель на сигнал по номеру в списке
     * \param num - номер сигнала
     */
    ChannelData* getSignal(const int num) const;

    /*!
     * \brief Возвращает указатель на сигнал по id сигнала
     * \param channelId - id сигнала
     */
    ChannelData* getSignal(const QString &channelId) const;

    /*!
     * \brief Добавляет сигнал к списку пробы.
     * Вызывать сервисным модулям НЕЛЬЗЯ!!!
     * \param signal - добавляемый сигнал
     */
    void addChannel(ChannelData* signal);

private:
    QString m_testUid;
    QString m_uid;
    QString m_name;
    QList<ChannelData*> m_channels;
};

#endif // PROBERESULTDATA_H
