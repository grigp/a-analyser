#ifndef COMPUTEFFT_H
#define COMPUTEFFT_H

#include "fftw3.h"

#include <QVector>

namespace ComputeFFT
{

static const int FFT_COUNT = 1024;

/*!
 * \brief Базовая функция расчета спектра.
 * Содержит базовый алгоритм расчета.
 * \param data - данные для расчета спектра. На входе содержит сигнал, на выходе - спектр. Размер должен быть кратен степени двойки
 */
bool baseFFT(QVector<double> &data);

}


class ComputeFFT_
{
public:
    ComputeFFT_();


private:

    void getBuffer(fftw_complex *bufferPtr, const int count);

};

#endif // COMPUTEFFT_H
