#include "trianglefactorsdescriptor.h"

#include "trianglefactors.h"

TriangleFactorsDescriptor::TriangleFactorsDescriptor(BaseDefines::TestLevel level)
    : MultiFactorDescriptor (level)
{
    TriangleFactors::registerFactors();
}

QString TriangleFactorsDescriptor::uid()
{
    return TriangleFactorsDefines::GroupUid;
}

QString TriangleFactorsDescriptor::name()
{
    return TriangleFactorsDefines::GroupName;
}

MultiFactor *TriangleFactorsDescriptor::caclulate(const QString &testUid, const QString &probeUid, const QString &channelUid)
{
    Q_UNUSED(channelUid);
    return new TriangleFactors(testUid, probeUid);
}
