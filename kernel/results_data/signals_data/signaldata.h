#ifndef SIGNALDATA_H
#define SIGNALDATA_H

#include <QString>

#include "channeldata.h"
#include "signalaccess.h"

/*!
 * \brief Базовый виртуальный класс сигнала SignalData class
 * Будет использоваться при отображении данных.
 * Доступ к данным унифицирован для любых сигналов
 */
class SignalData : public ChannelData, public SignalAccess
{
public:
    explicit SignalData();
    virtual ~SignalData();

    /*!
     * \brief Виртуальная функция, возвращающая идентификатор канала
     */
    virtual QString channelId() const = 0;

    /*!
     * \brief Виртуальная функция, возвращающая частоту дискретизации канала
     */
    int frequency() const override = 0;

    /*!
     * \brief Виртуальная функция, возвращающая размер сигнала в отсчетах
     */
    int size() const override = 0;

    /*!
     * \brief Виртуальная функция, возвращающая кол-во подканалов
     */
    int subChansCount() const override = 0;

    /*!
     * \brief Виртуальная функция, возвращающая данные канала
     * \param subChan - номер подканала (отведения)
     * \param rec - номер записи
     * \return значение сигнала п заданным номеру записи и номеру подканала
     */
    double value(const int subChan, const int rec) const override = 0;

    /*!
     * \brief Возвращает максимальное значение сигнала
     */
    virtual double maxValue() const;

    /*!
     * \brief Возвращает минимальное значение сигнала
     */
    virtual double minValue() const;

    /*!
     * \brief Возвращает максимальное значение сигнала по модулю
     */
    virtual double absMaxValue();

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

protected:
    double m_minValue {0};
    double m_maxValue {0};

};

#endif // SIGNALDATA_H
