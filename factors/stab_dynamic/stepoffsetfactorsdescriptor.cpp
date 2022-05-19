#include "stepoffsetfactorsdescriptor.h"

#include "stepoffsetfactors.h"

StepOffsetFactorsDescriptor::StepOffsetFactorsDescriptor(BaseDefines::TestLevel level)
    : MultiFactorDescriptor (level)
{
    StepOffsetFactors::registerFactors();
}

QString StepOffsetFactorsDescriptor::uid()
{
    return StepOffsetFactorsDefines::GroupUid;
}

QString StepOffsetFactorsDescriptor::name()
{
    return StepOffsetFactorsDefines::GroupName;
}

MultiFactor *StepOffsetFactorsDescriptor::caclulate(const QString &testUid, const QString &probeUid, const QString &channelUid)
{
    Q_UNUSED(channelUid);
    return new StepOffsetFactors(testUid, probeUid);
}
