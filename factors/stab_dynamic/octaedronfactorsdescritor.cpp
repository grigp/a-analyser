#include "octaedronfactorsdescritor.h"

#include "octaedronfactors.h"

OctaedronFactorsDescritior::OctaedronFactorsDescritior(BaseDefines::TestLevel level)
    : MultiFactorDescriptor(level)
{
    OctaedronFactors::registerFactors();
}

QString OctaedronFactorsDescritior::uid()
{
    return OctaedronFactorsDefines::GroupUid;
}

QString OctaedronFactorsDescritior::name()
{
    return OctaedronFactorsDefines::GroupName;
}

MultiFactor *OctaedronFactorsDescritior::caclulate(const QString &testUid, const QString &probeUid, const QString &channelUid)
{
    Q_UNUSED(channelUid);
    return new OctaedronFactors(testUid, probeUid);
}
