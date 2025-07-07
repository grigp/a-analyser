#include "spectrstabfactorsdescriptor.h"

#include "spectrstabfactors.h"

SpectrStabFactorsDescriptor::SpectrStabFactorsDescriptor(BaseDefines::TestLevel level)
    : MultiFactorDescriptor (level)
{
    SpectrStabFactors::registerFactors();
}

QString SpectrStabFactorsDescriptor::uid()
{
    return SpectrStabFactorsDefines::GroupUid;
}

QString SpectrStabFactorsDescriptor::name()
{
    return SpectrStabFactors::nameAsConst();
}

bool SpectrStabFactorsDescriptor::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    return SpectrStabFactors::isValid(testUid, probeUid, channelId);
}

MultiFactor *SpectrStabFactorsDescriptor::caclulate(const QString &testUid,
                                                    const QString &probeUid,
                                                    const QString &channelUid,
                                                    const int begin,
                                                    const int end)
{
    return new SpectrStabFactors(testUid, probeUid, channelUid, begin, end);
}
