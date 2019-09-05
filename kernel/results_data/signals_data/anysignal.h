#ifndef ANYSIGNAL_H
#define ANYSIGNAL_H

#include <QObject>
#include <QVector>

#include "signaldata.h"

/*!
 * \brief Класс сигнала общего назначения, формируемого вручную AnySignal class
 */
class AnySignal : public SignalData
{
public:
    explicit AnySignal(const int freq, const int subChans);

    /*!
     * \brief Перекрытая функция, возвращающая идентификатор канала
     */
    QString channelId() const override;

    /*!
     * \brief Перекрытая функция, возвращающая частоту дискретизации канала
     */
    int frequency() const override;

    /*!
     * \brief Перекрытая функция, возвращающая размер сигнала в отсчетах
     */
    int size() const override;

    /*!
     * \brief Перекрытая функция, возвращающая кол-во подканалов
     */
    int subChansCount() const override;

    /*!
     * \brief Перекрытая функция, возвращающая данные канала
     * \param subChan - номер подканала (отведения)
     * \param rec - номер записи
     * \return значение сигнала п заданным номеру записи и номеру подканала
     */
    double value(const int subChan, const int rec) const override;

    /*!
     * \brief добавляет значение
     */
    void appendValue(QVector<double> &value);

    /*!
     * \brief Читает данные из массива байт и заполняет буфер m_data
     * \param data - данные в виде массива байт
     */
    void fromByteArray(const QByteArray &data) override;

    /*!
     * \brief Записывает данные из m_data в массив байт
     * \param data - массив, в которые будут записаны данные
     */
    void toByteArray(QByteArray &data) const override;

    /*!
     * \brief Очищает данные
     */
    void clear() override;

private:
    int m_frequency;
    int m_subChansCount;

    QVector<QVector<double>> m_data;
};

#endif // ANYSIGNAL_H
