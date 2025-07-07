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
    return StepOffsetFactors::nameAsConst();
}

bool StepOffsetFactorsDescriptor::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    Q_UNUSED(channelId);
    return StepOffsetFactors::isValid(testUid, probeUid);
}

MultiFactor *StepOffsetFactorsDescriptor::caclulate(const QString &testUid,
                                                    const QString &probeUid,
                                                    const QString &channelUid,
                                                    const int begin,
                                                    const int end)
{
    Q_UNUSED(channelUid);
    Q_UNUSED(begin);
    Q_UNUSED(end);
    return new StepOffsetFactors(testUid, probeUid);
}
