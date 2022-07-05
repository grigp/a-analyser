#ifndef SIGNALACCESS_H
#define SIGNALACCESS_H

#include <QObject>

/*!
 * \brief Абстрактный класс, описывающий интерфейс доступа к сигналам SignalAccess class
 */
class SignalAccess
{
public:
    SignalAccess();

    virtual ~SignalAccess();

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
     * \brief Возвращает название подканала по номеру
     * \param i - номер подканала
     */
    virtual QString subChanName(const int i) const = 0;

    /*!
     * \brief Виртуальная функция, возвращающая данные канала
     * \param subChan - номер подканала (отведения)
     * \param rec - номер записи
     * \return значение сигнала п заданным номеру записи и номеру подканала
     */
    virtual double value(const int subChan, const int rec) const = 0;
};

#endif // SIGNALACCESS_H
