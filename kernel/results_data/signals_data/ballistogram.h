#ifndef BALLISTOGRAM_H
#define BALLISTOGRAM_H

#include "signaldata.h"
#include "signalsdefines.h"

#include <QVector>

/*!
 * \brief Класс сигнала баллистограммы Ballistogram class
 */
class Ballistogram : public SignalData
{
public:
    explicit Ballistogram(const QString &chanId, const int freq);

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
     * \brief Возвращает значение, как запсиь стабилограммы по номеру записи
     * \param rec - номер записи
     */
    double value(const int rec) const;

    /*!
     * \brief Добавляет значение
     * \param val - значение
     */
    void addValue(const double val);

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

private:
    QString m_channelId;
    int m_frequency;

    QVector<double> m_data;
};

#endif // BALLISTOGRAM_H
