#include "stepdeviationfactorsdescriptor.h"

#include "stepdeviationfactors.h"

StepDeviationFactorsDescriptor::StepDeviationFactorsDescriptor(BaseDefines::TestLevel level)
    : MultiFactorDescriptor (level)
{
    StepDeviationFactors::registerFactors();
}

QString StepDeviationFactorsDescriptor::uid()
{
    return StepDeviationFactorsDefines::GroupUid;
}

QString StepDeviationFactorsDescriptor::name()
{
    return StepDeviationFactorsDefines::GroupName;
}

bool StepDeviationFactorsDescriptor::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    Q_UNUSED(channelId);
    return StepDeviationFactors::isValid(testUid, probeUid);
}

MultiFactor *StepDeviationFactorsDescriptor::caclulate(const QString &testUid, const QString &probeUid, const QString &channelUid)
{
    Q_UNUSED(channelUid);
    return new StepDeviationFactors(testUid, probeUid);
}
