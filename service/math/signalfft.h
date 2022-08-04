#ifndef SIGNALFFT_H
#define SIGNALFFT_H

#include <QObject>
#include <QVector>

class SignalAccess;

/*!
 * \brief Класс, позволяющий рассчитывать спектр для сигнала по всем отведениям The SignalFFT class
 */
class SignalFFT
{
public:
    SignalFFT(SignalAccess *signal, const int points, const int step = -1);

    /*!
     * \brief Возвращает кол-во подканалов (рассчитанных спектров)
     */
    int channelsCount() const;

    /*!
     * \brief Возвращает кол-во точек в спектре
     */
    int points() const {return m_points / 2;}

    /*!
     * \brief Возвращает значение спектра
     * \param spectr - номер подканала
     * \param point - номер точки (0 .. m_points-1)
     */
    double value(const int spectr, const int point) const;

private:
    void calculateSpectr();

    SignalAccess *m_signal {nullptr};
    int m_points {1024};
    int m_step {-1};

    QList<QVector<double>> m_spectr;  ///< Рассчитанный спектр
};

#endif // SIGNALFFT_H
