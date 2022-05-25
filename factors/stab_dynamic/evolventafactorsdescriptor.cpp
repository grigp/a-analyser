#include "evolventafactorsdescriptor.h"

#include "evolventafactors.h"

EvolventaFactorsDescriptor::EvolventaFactorsDescriptor(BaseDefines::TestLevel level)
    : MultiFactorDescriptor (level)
{
    EvolventaFactors::registerFactors();
}

QString EvolventaFactorsDescriptor::uid()
{
    return EvolventaFactorsDefines::GroupUid;
}

QString EvolventaFactorsDescriptor::name()
{
    return EvolventaFactorsDefines::GroupName;
}

bool EvolventaFactorsDescriptor::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    Q_UNUSED(channelId);
    return EvolventaFactors::isValid(testUid, probeUid);
}

MultiFactor *EvolventaFactorsDescriptor::caclulate(const QString &testUid, const QString &probeUid, const QString &channelUid)
{
    Q_UNUSED(channelUid);
    return new EvolventaFactors(testUid, probeUid);
}
