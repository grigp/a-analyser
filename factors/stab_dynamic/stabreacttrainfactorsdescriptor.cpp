#include "stabreacttrainfactorsdescriptor.h"

#include "stabreacttrainfactors.h"

StabReactTrainFactorsDescriptor::StabReactTrainFactorsDescriptor(BaseDefines::TestLevel level)
    : MultiFactorDescriptor (level)
{
    StabReactTrainFactors::registerFactors();
}

QString StabReactTrainFactorsDescriptor::uid()
{
    return StabReactTrainFactorsDefines::GroupUid;
}

QString StabReactTrainFactorsDescriptor::name()
{
    return  StabReactTrainFactorsDefines::GroupName;
}

bool StabReactTrainFactorsDescriptor::isValid(const QString &testUid,
                                              const QString &probeUid,
                                              const QString &channelId)
{
    Q_UNUSED(channelId);
    return StabReactTrainFactors::isValid(testUid, probeUid);
}

MultiFactor *StabReactTrainFactorsDescriptor::caclulate(const QString &testUid,
                                                        const QString &probeUid,
                                                        const QString &channelUid,
                                                        const int begin, const int end)
{
    Q_UNUSED(channelUid);
    Q_UNUSED(begin);
    Q_UNUSED(end);
    return new StabReactTrainFactors(testUid, probeUid);
}
