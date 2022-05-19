#include "hoppingfactorsdescriptor.h"

#include "hoppingfactors.h"

HoppingFactorsDescriptor::HoppingFactorsDescriptor(BaseDefines::TestLevel level)
    : MultiFactorDescriptor (level)
{
    HoppingFactors::registerFactors();
}

QString HoppingFactorsDescriptor::uid()
{
    return HoppingFactorsDefines::GroupUid;
}

QString HoppingFactorsDescriptor::name()
{
    return HoppingFactorsDefines::GroupName;
}

MultiFactor *HoppingFactorsDescriptor::caclulate(const QString &testUid, const QString &probeUid, const QString &channelUid)
{
    return new HoppingFactors(testUid, probeUid, channelUid);
}
