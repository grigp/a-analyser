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
    return OctaedronFactors::nameAsConst();
}

bool OctaedronFactorsDescritior::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    Q_UNUSED(channelId);
    return OctaedronFactors::isValid(testUid, probeUid);
}

MultiFactor *OctaedronFactorsDescritior::caclulate(const QString &testUid,
                                                   const QString &probeUid,
                                                   const QString &channelUid,
                                                   const int begin,
                                                   const int end)
{
    Q_UNUSED(channelUid);
    Q_UNUSED(begin);
    Q_UNUSED(end);
    return new OctaedronFactors(testUid, probeUid);
}
