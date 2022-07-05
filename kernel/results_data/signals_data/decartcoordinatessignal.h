#ifndef DECARTCOORDINATESSIGNAL_H
#define DECARTCOORDINATESSIGNAL_H


#include "signaldata.h"
#include "signalsdefines.h"

#include <QVector>

/*!
 * \brief Класс сигнала декартовых координат на плоскости DecartCoordinatesSignal class
 */
class DecartCoordinatesSignal : public SignalData
{
public:
    explicit DecartCoordinatesSignal(const QString &chanId, const int freq);

    /*!
     * \brief Создает на основе массива байт
     * \param data - сигнал в виде массива байт
     */
    explicit DecartCoordinatesSignal(const QByteArray &data);

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
     * \brief Возвращает название подканала по номеру
     * \param i - номер подканала
     */
    QString subChanName(const int i) const override;

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
    SignalsDefines::StabRec value(const int rec) const;

    /*!
     * \brief Добавляет значение
     * \param rec - значение стабилограммы
     */
    void addValue(const SignalsDefines::StabRec rec);

    /*!
     * \brief Добавляет значение
     * \param x, y - координаты
     */
    void addValue(const double x, const double y);

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
    QString m_channelId;
    int m_frequency;

    QVector<SignalsDefines::StabRec> m_data;
};

#endif // DECARTCOORDINATESSIGNAL_H
