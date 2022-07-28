#include "computefft.h"

#include <QVector>
#include <complex>
#include <QDebug>

#include "baseutils.h"
#include "datadefines.h"



ComputeFFT_::ComputeFFT_()
{
    static const int FFT_COUNT = 1024;

    fftw_complex buffer[FFT_COUNT];
    getBuffer(&buffer[0], FFT_COUNT);
    QVector<double> src;
    for (int i = 0; i < FFT_COUNT; ++i)
        src << buffer[i][0];
    BaseUtils::vectorToText(src, DataDefines::aanalyserDocumentsPath() + "Export/fft_source.txt");

    fftw_plan plan = fftw_plan_dft_1d(FFT_COUNT, (fftw_complex*) &buffer[0], (fftw_complex*) &buffer[0], FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan);

    QVector<double> fft;
    for (int i = 0; i < FFT_COUNT / 2; ++i)
    {
        double* digit = (double*)&buffer[i];
        double value = 4 * sqrt(pow(digit[0], 2) + pow(digit[1], 2));
        fft << value;
    }
    BaseUtils::vectorToText(fft, DataDefines::aanalyserDocumentsPath() + "Export/fft_result.txt");

    fftw_destroy_plan(plan);

}

void ComputeFFT_::getBuffer(fftw_complex *bufferPtr, const int count)
{
    double *complexDigit;
    double r = 0;
    double r1 = 0;
    for(int i = 0; i < count; i++)
    {
        complexDigit = (double*)&bufferPtr[i];
        complexDigit[0] = 10 * sin(r) + 5 * sin(r1);	// re - реальная часть
        complexDigit[1] = 0;			// im - мнимая часть

        r += (2 * M_PI) / 51.2;
        r1 += (2 * M_PI) / 25.6;
    }
}

bool ComputeFFT::baseFFT(QVector<double> &data)
{
    const int size = data.size();
    //! Контроль на значение и степень двойки
    Q_ASSERT(((size >= 8) && ((size & (~size + 1)) == size)));

    fftw_complex buffer[size];
    double *complexDigit;
    for(int i = 0; i < size; i++)
    {
        complexDigit = (double*)&buffer[i];
        complexDigit[0] = data.at(i);	// re - реальная часть
        complexDigit[1] = 0;			// im - мнимая часть
    }

    fftw_plan plan = fftw_plan_dft_1d(size, (fftw_complex*) &buffer[0], (fftw_complex*) &buffer[0], FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan);

    for (int i = 0; i < size / 2; ++i)
    {
        double* digit = (double*)&buffer[i];
        double value = 4 * sqrt(pow(digit[0], 2) + pow(digit[1], 2));
        data.replace(i, value);
    }

    fftw_destroy_plan(plan);
}
