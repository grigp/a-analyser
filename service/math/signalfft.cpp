#include "signalfft.h"

#include <QDebug>

#include "signalaccess.h"
#include "computefft.h"


SignalFFT::SignalFFT(SignalAccess *signal, const int points, const int step)
    : m_signal(signal), m_points(points), m_step(step)
{
    calculateSpectr();
}

int SignalFFT::channelsCount() const
{
    return m_spectr.size();
}

double SignalFFT::value(const int spectr, const int point) const
{
    Q_ASSERT(spectr >= 0 && spectr < m_spectr.size());
    Q_ASSERT(point >= 0 && point < m_spectr.at(spectr).size());
    return m_spectr.at(spectr).at(point);
}

void SignalFFT::calculateSpectr()
{
    Q_ASSERT(m_signal);

    QList<QVector<double>> signal; //! Сигнал для спектра

    //! Инициализация буферов сигнала для расчета спектра
    for (int i = 0; i < m_signal->subChansCount(); ++i)
        signal << QVector<double>();

    //! Создание буферов сигнала для расчета спектра
    for (int i = 0; i < m_signal->size(); ++i)
        for (int j = 0; j < m_signal->subChansCount(); ++j)
        {
            auto sig = signal.at(j);
            sig << m_signal->value(j, i);
            signal.replace(j, sig);
        }

    //! Расчет спектра
    for (int j = 0; j < m_signal->subChansCount(); ++j)
    {
        QVector<double> spectr;
        auto sig = signal.at(j);
        bool b = ComputeFFT::extendFFT(sig, spectr, m_points, m_step);
        if (b)
            m_spectr << spectr;
    }
}
