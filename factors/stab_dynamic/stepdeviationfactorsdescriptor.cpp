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
    return StepDeviationFactors::nameAsConst();
}

bool StepDeviationFactorsDescriptor::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    Q_UNUSED(channelId);
    return StepDeviationFactors::isValid(testUid, probeUid);
}

MultiFactor *StepDeviationFactorsDescriptor::caclulate(const QString &testUid,
                                                       const QString &probeUid,
                                                       const QString &channelUid,
                                                       const int begin,
                                                       const int end)
{
    Q_UNUSED(channelUid);
    Q_UNUSED(begin);
    Q_UNUSED(end);
    return new StepDeviationFactors(testUid, probeUid);
}
