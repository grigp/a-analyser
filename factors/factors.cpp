#include "factors.h"

Factors::Factors(QString &testUid, QObject *parent)
    : QObject(parent)
    , m_testUid(testUid)
{
}

QString Factors::factorUid(const int id) const
{
    Q_ASSERT(id >= 0 && id < m_factors.size());
    return m_factors.at(id)->uid();
}

double Factors::factorValue(const int id) const
{
    Q_ASSERT(id >= 0 && id < m_factors.size());
    return m_factors.at(id)->value();
}

void Factors::addFactor(const QString &uid, const double value)
{
    m_factors.append(new FactorsDefines::FactorValue(uid, value));
}
