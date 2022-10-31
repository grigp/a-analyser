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
    explicit ApnoeFactors(QVector<double> signal, QObject *parent = nullptr);

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

signals:

public slots:

private:
    void calculate();
    void computeSemiWavesParams();
    void computeAmplitudes();

    QVector<double> m_signal;
    QList<SemiWave> m_semiWaves;
    double m_minAmpl {0};
    double m_midAmpl {0};
    double m_maxAmpl {0};
    double m_qAmpl {0};
};

#endif // APNOEFACTORS_H
