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

MultiFactor *StepDeviationFactorsDescriptor::caclulate(const QString &testUid, const QString &probeUid, const QString &channelUid)
{
    Q_UNUSED(channelUid);
    return new StepDeviationFactors(testUid, probeUid);
}
