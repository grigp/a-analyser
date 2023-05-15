#include "multifactor.h"

#include <QDebug>

#include "aanalyserapplication.h"

MultiFactor::MultiFactor(QObject *parent)
    : QObject(parent)
{
}

QString MultiFactor::factorUid(const int id) const
{
    Q_ASSERT(id >= 0 && id < m_factors.size());
    return m_factors.at(id)->uid();
}

double MultiFactor::factorValue(const int id) const
{
    Q_ASSERT(id >= 0 && id < m_factors.size());
    return m_factors.at(id)->value();
}

double MultiFactor::factorValue(const QString &uid) const
{
    foreach (auto fi, m_factors)
    {
        if (fi->uid() == uid)
            return fi->value();
    }
    return 0;
}

QString MultiFactor::factorValueFormatted(const QString &uid) const
{
    auto val = factorValue(uid);
    auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(uid);
    return  QString::number(val, 'f', fi.format());
}

bool MultiFactor::factorExists(const QString &uid) const
{
    foreach (auto fi, m_factors)
    {
        if (fi->uid() == uid)
            return true;
    }
    return false;
}

void MultiFactor::addFactor(const QString &uid, const double value)
{
    m_factors.append(new FactorsDefines::FactorValue(uid, value));
}

TestMultifactor::TestMultifactor(const QString &testUid, QObject *parent)
    : MultiFactor(parent)
    , m_testUid(testUid)
{

}

ProbeMultifactor::ProbeMultifactor(const QString &testUid, const QString &probeUid, QObject *parent)
    : TestMultifactor(testUid, parent)
    , m_probeUid(probeUid)
{

}

ChannelMultifactor::ChannelMultifactor(const QString &testUid,
                                       const QString &probeUid,
                                       const QString &channelId,
                                       const int begin,
                                       const int end,
                                       QObject *parent)
    : ProbeMultifactor(testUid, probeUid, parent)
    , m_channelId(channelId)
    , m_begin(begin)
    , m_end(end)
{

}
