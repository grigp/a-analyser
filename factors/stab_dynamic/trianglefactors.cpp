#include "trianglefactors.h"

#include <QDebug>

#include "channelsdefines.h"
#include "dataprovider.h"
#include "stabilogram.h"
#include "triangleresultdata.h"

TriangleFactors::TriangleFactors(const QString &testUid,
                                 const QString &probeUid,
                                 QObject *parent)
    : ProbeMultifactor (testUid, probeUid, parent)
{
    if (isValid())
        calculate();
}

bool TriangleFactors::isValid() const
{
    return isValid(testUid(), probeUid());
}

bool TriangleFactors::isValid(const QString &testUid, const QString &probeUid)
{
    Q_UNUSED(testUid);
    auto de = DataProvider::channelExists(probeUid, ChannelsDefines::chanStab);
    auto se = DataProvider::channelExists(probeUid, ChannelsDefines::chanTriangleResult);
    return de && se;
}

void TriangleFactors::calculate()
{
    getTriangleData();
    readSignal();
    computeTrianglesBounds();
    computeTriangles();
}

void TriangleFactors::registerFactors()
{

}

int TriangleFactors::freq() const
{
    if (m_resData)
        return m_resData->freq();
    return 128;
}

int TriangleFactors::diap() const
{
    if (m_resData)
        return m_resData->diap();
    return 128;
}

int TriangleFactors::trainingLength() const
{
    if (m_resData)
        return m_resData->trainingLength();
    return 0;
}

int TriangleFactors::signalLength() const
{
    return m_x.size();
}

TriangleDefines::Triangle TriangleFactors::triangleOriginal()
{
    if (m_resData)
        return TriangleDefines::Triangle(m_resData->topCorner(),
                                         m_resData->leftDownCorner(),
                                         m_resData->rightDownCorner());
    return TriangleDefines::Triangle(QPointF(), QPointF(), QPointF());
}

int TriangleFactors::trianglesCount() const
{
    return m_triangleSections.size();
}

BaseUtils::Section TriangleFactors::triangleSection(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < m_triangleSections.size());
    return m_triangleSections.at(idx);
}

TriangleDefines::Triangle TriangleFactors::triangle(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < m_triangles.size());
    return m_triangles.at(idx);
}

void TriangleFactors::readSignal()
{
    m_x.clear();
    m_xf.clear();
    m_y.clear();
    m_yf.clear();

    QByteArray baStab;
    if (DataProvider::getChannel(probeUid(), ChannelsDefines::chanStab, baStab))
    {
        Stabilogram stab(baStab);
        m_freq = stab.frequency();

        for (int i = 0; i < stab.size(); ++i)
        {
            double x = stab.value(0, i);
            double y = stab.value(1, i);
            m_x << x;
            m_y << y;
            m_xf << x;
            m_yf << y;
        }

        //! Фильтрация
        auto filtration = [&](QVector<double> &buffer)
        {
            BaseUtils::filterLowFreq(buffer, m_freq, 0.2, BaseUtils::fkChebyshev, 0, buffer.size() - 1);
            BaseUtils::swapVector(buffer);
            BaseUtils::filterLowFreq(buffer, m_freq, 0.2, BaseUtils::fkChebyshev, 0, buffer.size() - 1);
            BaseUtils::swapVector(buffer);
        };

        filtration(m_xf);
        filtration(m_yf);

//        BaseUtils::vectorToText(m_x, "c:/1/x.txt");
//        BaseUtils::vectorToText(m_xf, "c:/1/x_f.txt");
//        BaseUtils::vectorToText(m_y, "c:/1/y.txt");
//        BaseUtils::vectorToText(m_yf, "c:/1/y_f.txt");
    }
}

void TriangleFactors::computeTrianglesBounds()
{
    m_triangleSections.clear();

    int dir = -2;
    int begin = -1;
    int end = -1;
    for (int i = 0; i < m_yf.size(); ++i)
        if (i > 0)
        {
            if ((m_yf.at(i) < m_yf.at(i - 1)) && (dir == 0 || dir == 1))
            {
                end = i;
                if (begin > -1 && end > -1)
                    m_triangleSections << BaseUtils::Section(begin, end);
                //! Первый треугольник на этапе анализа
                if (begin < m_resData->trainingLength() && end >= m_resData->trainingLength())
                    m_firstAnalysisTriangle = m_triangleSections.size();
                if (begin == m_resData->trainingLength())
                    m_firstAnalysisTriangle = m_triangleSections.size() - 1;
                begin = i;
            }

            if (m_yf.at(i) > m_yf.at(i - 1))
                dir = 1;
            else
            if (m_yf.at(i) < m_yf.at(i - 1))
                dir = -1;
            else
                dir = 0;
        }
}

void TriangleFactors::computeTriangles()
{
    foreach (auto section, m_triangleSections)
    {
        //! Векторы: исходный и повернутые на +120 и -120 градусов
        QVector<QPointF> vn;
        QVector<QPointF> vrr;
        QVector<QPointF> vrl;
        for (int i = section.begin; i < section.end; ++i)
        {
            double x = m_x.at(i);
            double y = m_y.at(i);
            vn << QPointF(x, y);

            double xr = x * cos(2 * M_PI / 3) + y * sin(2 * M_PI / 3);
            double yr = - x * sin(2 * M_PI / 3) + y * cos(2 * M_PI / 3);
            vrr << QPointF(xr, yr);

            xr = x * cos(- 2 * M_PI / 3) + y * sin(- 2 * M_PI / 3);
            yr = - x * sin(- 2 * M_PI / 3) + y * cos(- 2 * M_PI / 3);
            vrl << QPointF(xr, yr);
        }

        //! Расчет координат вершин
        auto t = computeCorner(vn);
        auto ld = computeCorner(vrl);
        auto rd = computeCorner(vrr);

        //! Заполнение массива координат вершин
        m_triangles << TriangleDefines::Triangle(t, ld, rd);
    }
}

bool coordinateTopLessThan(QPointF &p1, QPointF &p2)
{
    return p1.y() < p2.y();
}

QPointF TriangleFactors::computeCorner(QVector<QPointF> &stab)
{
    //! Сортируем массив
    std::sort(stab.begin(), stab.end(), coordinateTopLessThan);

}

void TriangleFactors::getTriangleData()
{
    QByteArray baData;
    if (DataProvider::getChannel(probeUid(), ChannelsDefines::chanTriangleResult, baData))
        m_resData = new TriangleResultData(baData);
}
