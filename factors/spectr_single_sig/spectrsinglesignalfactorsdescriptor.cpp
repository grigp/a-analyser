#include "spectrsinglesignalfactorsdescriptor.h"

#include "spectrsinglesignalfactors.h"

SpectrSingleSignalFactorsDescriptor::SpectrSingleSignalFactorsDescriptor(BaseDefines::TestLevel level)
    :MultiFactorDescriptor (level)
{
    SpectrSingleSignalFactors::registerFactors();
}

QString SpectrSingleSignalFactorsDescriptor::uid()
{
    return SpectrSingleSignalFactorsDefines::GroupUid;
}

QString SpectrSingleSignalFactorsDescriptor::name()
{
    return SpectrSingleSignalFactors::nameAsConst();
}

bool SpectrSingleSignalFactorsDescriptor::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    return SpectrSingleSignalFactors::isValid(testUid, probeUid, channelId);
}

MultiFactor *SpectrSingleSignalFactorsDescriptor::caclulate(const QString &testUid,
                                                            const QString &probeUid,
                                                            const QString &channelUid,
                                                            const int begin,
                                                            const int end)
{
    return new SpectrSingleSignalFactors(testUid, probeUid, channelUid, begin, end);
}
