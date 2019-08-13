#ifndef SIGNALDATA_H
#define SIGNALDATA_H

#include <QString>

/*!
 * \brief Базовый виртуальный класс сигнала Signal class
 * Будет использоваться при отображении данных.
 * Доступ к данным унифицирован для любых сигналов
 */
class SignalData
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
    virtual int frequency() const = 0;

    /*!
     * \brief Виртуальная функция, возвращающая размер сигнала в отсчетах
     */
    virtual int size() const = 0;

    /*!
     * \brief Виртуальная функция, возвращающая кол-во подканалов
     */
    virtual int subChansCount() const = 0;

    /*!
     * \brief Виртуальная функция, возвращающая данные канала
     * \param subChan - номер подканала (отведения)
     * \param rec - номер записи
     * \return значение сигнала п заданным номеру записи и номеру подканала
     */
    virtual double value(const int subChan, const int rec) const = 0;

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

protected:
    double m_minValue {0};
    double m_maxValue {0};

};

#endif // SIGNALDATA_H
