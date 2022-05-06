#ifndef MYOGRAM_H
#define MYOGRAM_H

#include <QVector>

#include "signaldata.h"
#include "signalsdefines.h"


class Myogram : public SignalData
{
public:
    explicit Myogram(const QString &chanId, const int subChansCount, const int freq);

    /*!
     * \brief Создает на основе массива байт
     * \param data - сигнал в виде массива байт
     */
    explicit Myogram(const QByteArray &data);

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
     * \brief Установлние кол-ва подканалов
     * Возможно, если только не было добавлений новых записей addValue или прочитано канал fromByteArray
     * \param scc - новое кол-во
     */
    void setSubChansCount (const int scc);

    /*!
     * \brief Перекрытая функция, возвращающая данные канала
     * \param subChan - номер подканала (отведения)
     * \param rec - номер записи
     * \return значение сигнала п заданным номеру записи и номеру подканала
     */
    double value(const int subChan, const int rec) const override;

    /*!
     * \brief Возвращает значение, как запись миограммы по номеру записи
     * \param rec - номер записи
     */
    QVector<double> value(const int rec) const;

    /*!
     * \brief Добавляет значение
     * \param rec - значение миограммы
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
    int m_subChansCount {0};
    int m_frequency {0};

    bool m_isDataAdded {false};

    QVector<SignalsDefines::MyoRec> m_data;
};

#endif // MYOGRAM_H
