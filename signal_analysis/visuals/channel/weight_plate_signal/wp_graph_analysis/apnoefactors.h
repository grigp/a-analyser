#ifndef APNOEFACTORS_H
#define APNOEFACTORS_H

#include <QObject>
#include <QVector>

/*!
 * \brief Класс для расчет показателей апноэ для сигнала The ApnoeFactors class
 */
class ApnoeFactors : public QObject
{
    Q_OBJECT
public:
    explicit ApnoeFactors(QVector<double> signal, const int frequency, QObject *parent = nullptr);

    enum ExtremumKind
    {
          ekUndefined = 0
        , ekMinimum
        , ekMaximum
    };

    /*!
     * \brief Переход между экстремумами The SemiWave struct
     */
    struct SemiWave
    {
        int begin {0};
        int end {0};
        double amplitude {0};
        ExtremumKind kind {ekUndefined};
        SemiWave(const int b, const int e, const double a, const ExtremumKind k)
            : begin(b), end(e), amplitude(a), kind(k){}
    };

    /*!
     * \brief Возвращает кол-во полуволн дыхания
     */
    int semiWavesCount() const;

    /*!
     * \brief Возвращает параметры полуволны из списка по индексу
     * \param idx - индекс полуволны
     */
    SemiWave semiWave(const int idx) const;

    /*!
     * \brief Кол-во приступов апноэ
     */
    int apnoeFactsCount() const {return m_apnoeFactsCount;}

    /*!
     * \brief Средняя продолжительность приступа апноэ
     */
    double apnoeFactTimeAverage() {return m_apnoeFactTimeAverage;}

    /*!
     * \brief Максимальная продолжительность приступа апноэ
     */
    double apnoeFactTimeMax() {return m_apnoeFactTimeMax;}


signals:

public slots:

private:
    void calculate();
    void computeSemiWavesParams();
    void computeAmplitudes();

    QVector<double> m_signal;
    int m_frequency {50};
    QList<SemiWave> m_semiWaves;
    double m_minAmpl {0};
    double m_midAmpl {0};
    double m_maxAmpl {0};
    double m_qAmpl {0};

    int m_apnoeFactsCount {0};          ///< Кол-во приступов апноэ
    double m_apnoeFactTimeAverage {0};  ///< Средняя продолжительность приступа апноэ
    double m_apnoeFactTimeMax {0};      ///< Максимальная продолжительность приступа апноэ
};

#endif // APNOEFACTORS_H
