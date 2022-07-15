#include "computefft.h"

#include <QVector>
#include "complex.h"
#include <QDebug>

#include "fftw3.h"



ComputeFFT::ComputeFFT()
{
    // создаем одномерную выборку, все значения которой равны 1
//    QVector<complex<double> > data(64, 1.);
    QVector<double> data(64, 0.);
    double r = 0;
    qDebug() << "---------------------------------------------";
    for (int i = 0; i < 64; ++i)
    {
        data[i] = 10 * sin(r);
        qDebug() << data[i];
        r = r + (2 * M_PI) / 64;
//        r = r + 2 / (2 * M_PI);
    }
    qDebug() << "---------------------------------------------";

    // создаем план для библиотеки fftw
    fftw_plan plan = fftw_plan_dft_1d(data.size(), (fftw_complex*) &data[0], (fftw_complex*) &data[0], FFTW_FORWARD, FFTW_ESTIMATE);
//    fftw_plan plan = fftw_plan_dft_1d(data.size(), static_cast<fftw_complex*>(&data[0]), (fftw_complex*) &data[0], FFTW_FORWARD, FFTW_ESTIMATE);

    // преобразование Фурье
    fftw_execute(plan);
    fftw_destroy_plan(plan);

    // выводим в файл результат преобразования Фурье (должна получиться Дельта-функция)
//    ofstream out("speсtr.txt");
//    for(size_t i=0; i < data.size(); ++i)
    for(auto i = 0; i < data.size(); ++i)
    {
        qDebug() << data[i];
//       out<<data[i]<<endl;
    }
}
