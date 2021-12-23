#include "trianglecalculator.h"

#include "trianglefactors.h"
#include "datadefines.h"
#include "channelsdefines.h"
#include "dataprovider.h"

TriangleCalculator::TriangleCalculator(const QString &testUid, QObject *parent)
    : TestCalculator (testUid, parent)
{

}

void TriangleCalculator::calculate()
{
    TestCalculator::calculate();

    //! Необходимо в тесте, в котором будет возможно ни одна проба найти нужную пробу
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid(), ti))
    {
        for (int i = 0; i < ti.probes.size(); ++i)
        {
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(ti.probes.at(i), pi))
            {
                if (DataProvider::channelExists(pi.uid, ChannelsDefines::chanStab) &&
                    DataProvider::channelExists(pi.uid, ChannelsDefines::chanTriangleResult))
                {
                    m_factors = new TriangleFactors(testUid(), pi.uid);

//                    for (int i = 0; i < m_factors->size(); ++i)
//                    {
//                        addPrimaryFactor(testUid(), m_factors->factorUid(i),
//                                         m_factors->factorValue(m_factors->factorUid(i)),
//                                         0, ChannelsDefines::chanStab, pi.name);
//                    }
                }
            }
        }
    }
}

void TriangleCalculator::fastCalculate()
{
    TestCalculator::fastCalculate();
}

int TriangleCalculator::freq() const
{
    if (m_factors)
        return m_factors->freq();
    return 50;
}

int TriangleCalculator::diap() const
{
    if (m_factors)
        return m_factors->diap();
    return 128;
}

int TriangleCalculator::trainingLength() const
{
    if (m_factors)
        return m_factors->trainingLength();
    return 0;
}

int TriangleCalculator::firstAnalysisTriangle() const
{
    if (m_factors)
        return  m_factors->firstAnalysisTriangle();
    return 0;
}

int TriangleCalculator::signalLength() const
{
    if (m_factors)
        return m_factors->signalLength();
    return 0;
}

TriangleDefines::Triangle TriangleCalculator::triangleOriginal()
{
    if (m_factors)
        return m_factors->triangleOriginal();
    return TriangleDefines::Triangle(QPointF(), QPointF(), QPointF());
}

int TriangleCalculator::trianglesCount() const
{
    if (m_factors)
        return m_factors->trianglesCount();
    return 0;
}

BaseUtils::Section TriangleCalculator::triangleSection(const int idx) const
{
    if (m_factors)
        return m_factors->triangleSection(idx);
    return BaseUtils::Section(0, 0);
}

TriangleDefines::Triangle TriangleCalculator::triangle(const int idx) const
{
    if (m_factors)
        return m_factors->triangle(idx);
    return TriangleDefines::Triangle(QPointF(), QPointF(), QPointF());
}
