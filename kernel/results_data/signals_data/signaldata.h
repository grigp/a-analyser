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
};

#endif // SIGNALDATA_H
