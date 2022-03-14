#ifndef WEIGHTPLATESIGNAL_H
#define WEIGHTPLATESIGNAL_H

#include "signaldata.h"
#include "signalsdefines.h"

#include <QVector>

/*!
 * \brief Класс сигналов весовой платформы The WeightPlateSignal class
 */
class WeightPlateSignal : public SignalData
{
public:
    explicit WeightPlateSignal(const QString &chanId, const int freq, const int subChansCount);

    /*!
     * \brief Создает на основе массива байт
     * \param data - сигнал в виде массива байт
     */
    explicit WeightPlateSignal(const QByteArray &data);

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
     * \brief Добавляет значение
     * \param rec - значение
     */
    void addValue(const QVector<double> rec);

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
    QString m_channelId {""};
    int m_frequency {50};
    int m_subChansCount {1};

    QVector<QVector<double>> m_data;
};

#endif // WEIGHTPLATESIGNAL_H
