#include "trenresultfactorsdescriptor.h"

#include "trenresultfactors.h"

TrenResultFactorsDescriptor::TrenResultFactorsDescriptor(BaseDefines::TestLevel level)
    : MultiFactorDescriptor (level)
{
    TrenResultFactors::registerFactors();
}

QString TrenResultFactorsDescriptor::uid()
{
    return TrenResultFactorsDefines::GroupUid;
}

QString TrenResultFactorsDescriptor::name()
{
    return TrenResultFactorsDefines::GroupName;
}

bool TrenResultFactorsDescriptor::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    return TrenResultFactors::isValid(testUid, probeUid, channelId);
}

MultiFactor *TrenResultFactorsDescriptor::caclulate(const QString &testUid,
                                                    const QString &probeUid,
                                                    const QString &channelUid,
                                                    const int begin,
                                                    const int end)
{
    Q_UNUSED(channelUid);
    Q_UNUSED(begin);
    Q_UNUSED(end);
    return new TrenResultFactors(testUid, probeUid);
}
