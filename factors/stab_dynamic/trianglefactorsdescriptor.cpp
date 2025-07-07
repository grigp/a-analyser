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
    return TriangleFactors::nameAsConst();
}

bool TriangleFactorsDescriptor::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    Q_UNUSED(channelId);
    return TriangleFactors::isValid(testUid, probeUid);
}

MultiFactor *TriangleFactorsDescriptor::caclulate(const QString &testUid,
                                                  const QString &probeUid,
                                                  const QString &channelUid,
                                                  const int begin,
                                                  const int end)
{
    Q_UNUSED(channelUid);
    Q_UNUSED(begin);
    Q_UNUSED(end);
    return new TriangleFactors(testUid, probeUid);
}
