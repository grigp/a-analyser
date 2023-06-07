#include "pulsespectrfactorsdescriptor.h"

#include "pulsespectrfactors.h"

PulseSpectrFactorsDescriptor::PulseSpectrFactorsDescriptor(BaseDefines::TestLevel level)
    : MultiFactorDescriptor (level)
{
    PulseSpectrFactors::registerFactors();
}

QString PulseSpectrFactorsDescriptor::uid()
{
    return PulseSpectrFactorsDefines::GroupUid;
}

QString PulseSpectrFactorsDescriptor::name()
{
    return PulseSpectrFactorsDefines::GroupName;
}

bool PulseSpectrFactorsDescriptor::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    return PulseSpectrFactors::isValid(testUid, probeUid, channelId);
}

MultiFactor *PulseSpectrFactorsDescriptor::caclulate(const QString &testUid,
                                                     const QString &probeUid,
                                                     const QString &channelUid,
                                                     const int begin, const int end)
{
    return new PulseSpectrFactors(testUid, probeUid, channelUid, begin, end);
}
