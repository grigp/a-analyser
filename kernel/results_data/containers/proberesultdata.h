#ifndef PROBERESULTDATA_H
#define PROBERESULTDATA_H

#include <QList>

#include "signaldata.h"

class ProbeResultData
{
public:
    explicit ProbeResultData(const QString &uid, const QString &name);

    /*!
     * \brief Возвращает uid пробы
     */
    QString uid();

    /*!
     * \brief Возвращает название пробы
     */
    QString name();

    /*!
     * \brief Возвращает кол-во сигналов в пробе
     */
    int signalsCount();

    /*!
     * \brief Возвращает указатель на сигнал по номеру в списке
     * \param num - номер сигнала
     */
    SignalData* getSignal(const int num);

    /*!
     * \brief Возвращает указатель на сигнал по id сигнала
     * \param channelId - id сигнала
     */
    SignalData* getSignal(const QString &channelId);

private:
    QString m_uid;
    QString m_name;
    QList<SignalData*> m_signals;
};

#endif // PROBERESULTDATA_H
