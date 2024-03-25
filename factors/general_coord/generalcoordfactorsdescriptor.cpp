#include "generalcoordfactorsdescriptor.h"

#include "generalcoordfactors.h"

GeneralCoordFactorsDescriptor::GeneralCoordFactorsDescriptor(BaseDefines::TestLevel level)
    : MultiFactorDescriptor (level)
{
    GeneralCoordFactors::registerFactors();
}

QString GeneralCoordFactorsDescriptor::uid()
{
    return GeneralCoordFactorsDefines::GroupUid;
}

QString GeneralCoordFactorsDescriptor::name()
{
    return GeneralCoordFactorsDefines::GroupName;
}

bool GeneralCoordFactorsDescriptor::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    Q_UNUSED(testUid);
    Q_UNUSED(probeUid);
    Q_UNUSED(channelId);
    return false;
}

MultiFactor *GeneralCoordFactorsDescriptor::caclulate(const QString &testUid, const QString &probeUid, const QString &channelUid, const int begin, const int end)
{
    Q_UNUSED(testUid);
    Q_UNUSED(probeUid);
    Q_UNUSED(channelUid);
    Q_UNUSED(begin);
    Q_UNUSED(end);
    return nullptr;
}
