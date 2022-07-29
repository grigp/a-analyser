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
 * \return true, если успешно
 */
bool baseFFT(QVector<double> &data);

/*!
 * \brief Расширенная функкция расчета спектра
 * \param signal - исходный сигнал произвольной длины
 * \param spectr - рассчитанный спектр длиной count отсчетов
 * \param count - кол-во отсчетов спектра
 * \param step - шаг смещения по исходному сигналу
 * \return true, если успешно
 */
bool extendFFT(QVector<double> &signal, QVector<double> &spectr, const int count, const int step = -1);

}


//class ComputeFFT_
//{
//public:
//    ComputeFFT_();


//private:

//    void getBuffer(fftw_complex *bufferPtr, const int count);

//};

#endif // COMPUTEFFT_H
