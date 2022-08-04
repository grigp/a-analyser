#include "computefft.h"

#include <QVector>
#include <complex>
#include <QDebug>

#include "baseutils.h"
#include "datadefines.h"



//ComputeFFT_::ComputeFFT_()
//{
//    static const int FFT_COUNT = 1024;

//    fftw_complex buffer[FFT_COUNT];
//    getBuffer(&buffer[0], FFT_COUNT);
//    QVector<double> src;
//    for (int i = 0; i < FFT_COUNT; ++i)
//        src << buffer[i][0];
//    BaseUtils::vectorToText(src, DataDefines::aanalyserDocumentsPath() + "Export/fft_source.txt");

//    fftw_plan plan = fftw_plan_dft_1d(FFT_COUNT, (fftw_complex*) &buffer[0], (fftw_complex*) &buffer[0], FFTW_FORWARD, FFTW_ESTIMATE);
//    fftw_execute(plan);

//    QVector<double> fft;
//    for (int i = 0; i < FFT_COUNT / 2; ++i)
//    {
//        double* digit = (double*)&buffer[i];
//        double value = 4 * sqrt(pow(digit[0], 2) + pow(digit[1], 2));
//        fft << value;
//    }
//    BaseUtils::vectorToText(fft, DataDefines::aanalyserDocumentsPath() + "Export/fft_result.txt");

//    fftw_destroy_plan(plan);

//}

//void ComputeFFT_::getBuffer(fftw_complex *bufferPtr, const int count)
//{
//    double *complexDigit;
//    double r = 0;
//    double r1 = 0;
//    for(int i = 0; i < count; i++)
//    {
//        complexDigit = (double*)&bufferPtr[i];
//        complexDigit[0] = 10 * sin(r) + 5 * sin(r1);	// re - реальная часть
//        complexDigit[1] = 0;			// im - мнимая часть

//        r += (2 * M_PI) / 51.2;
//        r1 += (2 * M_PI) / 25.6;
//    }
//}

/*!
 * \brief Готовит данные для расчета спектра:
 * 1. Центровка
 * 2. Сглаживание краев окном Хемминга
 * \param data - подготавливаемые данные
 */
void prepareData(QVector<double> &data)
{
    //! Расчет среднего
    double mo = 0;
    foreach (auto v, data)
        mo += v;
    mo /= data.size();
    //! Усреднение
    for (int i = 0; i < data.size(); ++i)
        data.replace(i, data.at(i) - mo);

    //! Наложение окна - четверть периода синуса
    double r = 0;
    int n = data.size() / 10;
    for (int i = 0; i < n; ++i)
    {
        double fv = 0.5 + 0.5 * sin(-M_PI_2 + r);
        data.replace(i, data.at(i) * fv);
        data.replace(data.size() - i - 1, data.at(data.size() - i - 1) * fv);
        r += (2 * M_PI) / (n * 2);
    }
//    BaseUtils::vectorToText(data, DataDefines::aanalyserDocumentsPath() + "Export/fft_source_prepared.txt");
}

bool ComputeFFT::baseFFT(QVector<double> &data)
{
    const int size = data.size();
    //! Контроль на значение и степень двойки
    if ((size >= 8) && ((size & (~size + 1)) == size))
    {
        prepareData(data);

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
            double value = 4 * sqrt(pow(digit[0], 2) + pow(digit[1], 2)) / (size / 2);
            data.replace(i, value);
        }

        fftw_destroy_plan(plan);
        return true;
    }

    return false;
}

bool ComputeFFT::extendFFT(QVector<double> &signal, QVector<double> &spectr, const int count, const int step)
{
    //! Контроль на значение и степень двойки
    if (!((count >= 8) && ((count & (~count + 1)) == count)))
        return false;

    //! Если сигнал короче, чем 90% от заданного числа расчета, то считать не будем...
    if (signal.size() < count * 0.9)
        return false;

    //! Сигнал меньше указанного числа точек, например 20 сек при 50 Гц - 1000 точек, а count == 1024
    //! Дополним сигнал точками до 1024
    if (signal.size() < count)
    {
        //! Кол-во точек, которые надо добавить
        int d = count - signal.size();
        spectr.clear();
        //! Предшествующие точки
        for (int i = 0; i < d / 2; ++i)
            spectr << signal.at(0);
        //! Сам сигнал
        foreach (auto v, signal)
            spectr << v;
        //! Финальные точки
        while (spectr.size() < count)
            spectr << signal.at(signal.size() - 1);

//        BaseUtils::vectorToText(spectr, DataDefines::aanalyserDocumentsPath() + "Export/fft_source_prepared2.txt");

        //! Расчет спектра
        baseFFT(spectr);
    }
    else
    {
        if (step <= 0)
            return false;

        //! Начальная точка
        int n = 0;
        int start = 0;
        //! Буфер для усреднения
        spectr.clear();
        for (int i = 0; i < count; ++i)
            spectr << 0.0;

        //! Цикл, пока не выйдем за границы сигнала
        while (start + count < signal.size())
        {
            //! Подготовка данных в виде участка сигнала
            QVector<double> data;
            data.clear();
            for (int i = start; i < start + count; ++i)
                data << signal.at(i);

            //! Расчет спектра
            baseFFT(data);

            //! Усреднение - суммирование
            for (int i = 0; i < count; ++i)
                spectr.replace(i, spectr.at(i) + data.at(i));

            //! Сдвиг
            start += step;
            ++n;
        }

        //! Усреднение - деление
        for (int i = 0; i < count; ++i)
            spectr.replace(i, spectr.at(i) / n);
    }

    return true;
}
