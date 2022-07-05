#ifndef RESULTINFO_H
#define RESULTINFO_H

#include <QString>

#include "signalaccess.h"

class ProbeResultData;
class ChannelData;
class SignalResultInfo;


/*!
 * \brief Класс, ассоциированный с объектом пробы ProbeResultInfo class
 * Через него осуществляется доступ к данным пробы
 */
class ProbeResultInfo
{
public:
    explicit ProbeResultInfo(ProbeResultData* probe);

    /*!
     * \brief Возвращает кол-во сигналов в пробе
     */
    int signalsCount() const;

    /*!
     * \brief Возвращает указатель на сигнал по порядковому номеру
     * \param signalNumber - порядковый номер сигнала
     */
    SignalResultInfo *signal(const int signalNumber) const;

    /*!
     * \brief Возвращает указатель на сигнал по идентификатору канала
     * \param channelId - идентификатор канала
     */
    SignalResultInfo *signal(const QString &channelId) const;

    /*!
     * \brief Возвращает uid пробы
     */
    QString uid() const;

    /*!
     * \brief Возвращает название пробы
     */
    QString name() const;

private:
    ProbeResultData* m_probe;
};

/*!
 * \brief Класс, ассоциированный с объектом сигнала SignalResultInfo class
 * Через него осуществляется доступ к данным сигнала
 */
class SignalResultInfo : public SignalAccess
{
public:
    explicit SignalResultInfo(ChannelData* channel);

    /*!
     * \brief Возвращает идентификатор канала
     */
    QString channelId() const;

    /*!
     * \brief Возвращает частоту дискретизации сигнала
     */
    int frequency() const override;

    /*!
     * \brief Возвращает кол-во отсчетов сигнала
     */
    int size() const override;

    /*!
     * \brief Виртуальная функция, возвращающая кол-во подканалов
     */
    int subChansCount() const override;

    /*!
     * \brief Возвращает название подканала по номеру
     * \param i - номер подканала
     */
    QString subChanName(const int i) const override;

    /*!
     * \brief Возвращает значение сигнала
     * \param chan - номер канала
     * \param rec - номер записи
     */
    double value(const int chan, const int rec) const override;

    /*!
     * \brief Возвращает максимальное значение сигнала по модулю
     */
    double absMaxValue();

private:
    ChannelData* m_channel;
};

#endif // RESULTINFO_H
