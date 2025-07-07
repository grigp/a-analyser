#include "mwstickforcefactorsdescriptor.h"

#include "mwstickforcefactors.h"

MWStickForceFactorsDescriptor::MWStickForceFactorsDescriptor(BaseDefines::TestLevel level)
    : MultiFactorDescriptor(level)
{
    MWStickForceFactors::registerFactors();
}

QString MWStickForceFactorsDescriptor::uid()
{
    return MWStickForceFactorsDefines::GroupUid;
}

QString MWStickForceFactorsDescriptor::name()
{
    return MWStickForceFactors::nameAsConst();
}

bool MWStickForceFactorsDescriptor::isValid(const QString &testUid,
                                            const QString &probeUid,
                                            const QString &channelId)
{
    return MWStickForceFactors::isValid(testUid, probeUid, channelId);
}

MultiFactor *MWStickForceFactorsDescriptor::caclulate(const QString &testUid,
                                                      const QString &probeUid,
                                                      const QString &channelUid,
                                                      const int begin, const int end)
{
    return new MWStickForceFactors(testUid, probeUid, channelUid, begin, end);
}
