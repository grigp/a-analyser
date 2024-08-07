#include "trianglecalculator.h"

#include <QUuid>

#include "trianglefactors.h"
#include "triangleconslutionfactors.h"
#include "datadefines.h"
#include "channelsdefines.h"
#include "dataprovider.h"

namespace
{
  QList<QString> PrimaryFactors =
  {
        QString(TriangleFactorsDefines::Training::MXUid)
      , QString(TriangleFactorsDefines::Training::MYUid)
      , QString(TriangleFactorsDefines::Training::TimeUid)
      , QString(TriangleFactorsDefines::Training::SquareUid)
      , QString(TriangleFactorsDefines::Training::SpeedUid)
      , QString(TriangleFactorsDefines::Training::MotorPwrUid)
      , QString(TriangleFactorsDefines::Training::KognPwrUid)
      , QString(TriangleFactorsDefines::Analysis::MXUid)
      , QString(TriangleFactorsDefines::Analysis::MYUid)
      , QString(TriangleFactorsDefines::Analysis::TimeUid)
      , QString(TriangleFactorsDefines::Analysis::SquareUid)
      , QString(TriangleFactorsDefines::Analysis::SpeedUid)
      , QString(TriangleFactorsDefines::Analysis::MotorPwrUid)
      , QString(TriangleFactorsDefines::Analysis::KognPwrUid)
      , QString(TriangleConslutionFactorsDefines::KorrDominTstUid)
      , QString(TriangleConslutionFactorsDefines::KorrDominAnlUid)
      , QString(TriangleFactorsDefines::LatentMovingUid)
  };

}


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
                    m_factorsConsl = new TriangleConslutionFactors(testUid(), pi.uid, m_factors);

                    foreach (auto uid, PrimaryFactors)
                    {
                        if (m_factors->factorExists(uid))
                            addPrimaryFactor(testUid(), uid, m_factors->factorValue(uid), 0, ChannelsDefines::chanStab, pi.name);
                        else
                        if (m_factorsConsl->factorExists(uid))
                            addPrimaryFactor(testUid(), uid, m_factorsConsl->factorValue(uid), 0, ChannelsDefines::chanStab, pi.name);
                    }
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

TriangleDefines::Triangle TriangleCalculator::triangleTraining()
{
    if (m_factors)
        return m_factors->triangleTraining();
    return TriangleDefines::Triangle(QPointF(), QPointF(), QPointF());
}

TriangleDefines::Triangle TriangleCalculator::triangleAnalysis()
{
    if (m_factors)
        return m_factors->triangleAnalysis();
    return TriangleDefines::Triangle(QPointF(), QPointF(), QPointF());
}

int TriangleCalculator::trianglesCount() const
{
    if (m_factors)
        return m_factors->trianglesCount();
    return 0;
}

BaseDefines::Section TriangleCalculator::triangleSection(const int idx) const
{
    if (m_factors)
        return m_factors->triangleSection(idx);
    return BaseDefines::Section(0, 0);
}

TriangleDefines::Triangle TriangleCalculator::triangle(const int idx) const
{
    if (m_factors)
        return m_factors->triangle(idx);
    return TriangleDefines::Triangle(QPointF(), QPointF(), QPointF());
}

int TriangleCalculator::factorCount() const
{
    if (m_factors)
        return  m_factors->size();
    return 0;
}

QString TriangleCalculator::factorUid(const int id) const
{
    if (m_factors)
        return  m_factors->factorUid(id);
    return QUuid().toString();
}

double TriangleCalculator::factorValue(const int id) const
{
    if (m_factors)
        return  m_factors->factorValue(id);
    return 0;
}

double TriangleCalculator::factorValue(const QString &uid) const
{
    if (m_factors)
        return  m_factors->factorValue(uid);
    return 0;
}

QString TriangleCalculator::factorValueFormatted(const QString &uid) const
{
    if (m_factors)
        return  m_factors->factorValueFormatted(uid);
    return "0";
}

int TriangleCalculator::getFactorsOfStagesCount()
{
    if (m_factors)
        return m_factors->getFactorsOfStagesCount();
    return 0;
}

bool TriangleCalculator::getFactorOfStages(const int idx, QString &uidT, QString &uidA)
{
    if (m_factors)
        return m_factors->getFactorOfStages(idx, uidT, uidA);
    return false;
}

double TriangleCalculator::factorConslutionValue(const QString &uid) const
{
    if (m_factorsConsl)
        return m_factorsConsl->factorValue(uid);
    return 0;
}

QString TriangleCalculator::factorConslutionValueFormatted(const QString &uid) const
{
    if (m_factorsConsl)
        return m_factorsConsl->factorValueFormatted(uid);
    return "";
}
