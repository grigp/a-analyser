#ifndef RESULTINFO_H
#define RESULTINFO_H

#include <QString>

class ProbeResultData;
class SignalData;
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
class SignalResultInfo
{
public:
    explicit SignalResultInfo(SignalData* signal);

    /*!
     * \brief Возвращает идентификатор канала
     */
    QString channelId() const;

    /*!
     * \brief Возвращает частоту дискретизации сигнала
     */
    int frequency() const;

    /*!
     * \brief Возвращает кол-во отсчетов сигнала
     */
    int size() const;

    /*!
     * \brief Возвращает значение сигнала
     * \param chan - номер канала
     * \param rec - номер записи
     */
    double value(const int chan, const int rec) const;

    /*!
     * \brief Возвращает максимальное значение сигнала по модулю
     */
    double absMaxValue();

private:
    SignalData* m_signal;
};

#endif // RESULTINFO_H
