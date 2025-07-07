#include "triangleconslutionfactorsdescriptor.h"

#include "triangleconslutionfactors.h"

TriangleConslutionFactorsDescriptor::TriangleConslutionFactorsDescriptor(BaseDefines::TestLevel level)
    : MultiFactorDescriptor (level)
{
    TriangleConslutionFactors::registerFactors();
}

QString TriangleConslutionFactorsDescriptor::uid()
{
    return TriangleConslutionFactorsDefines::GroupUid;
}

QString TriangleConslutionFactorsDescriptor::name()
{
    return TriangleConslutionFactors::nameAsConst();
}

bool TriangleConslutionFactorsDescriptor::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    Q_UNUSED(channelId);
    return TriangleConslutionFactors::isValid(testUid, probeUid);
}

MultiFactor *TriangleConslutionFactorsDescriptor::caclulate(const QString &testUid,
                                                            const QString &probeUid,
                                                            const QString &channelUid,
                                                            const int begin,
                                                            const int end)
{
    Q_UNUSED(channelUid);
    Q_UNUSED(begin);
    Q_UNUSED(end);
    return new TriangleConslutionFactors(testUid, probeUid);
}
