#include "boxerdodgingmultifactordescriptor.h"

#include "boxerdodgingmultifactor.h"

BoxerDodgingMultifactorDescriptor::BoxerDodgingMultifactorDescriptor(BaseDefines::TestLevel level)
    : MultiFactorDescriptor (level)
{
    BoxerDodgingMultifactor::registerFactors();
}

QString BoxerDodgingMultifactorDescriptor::uid()
{
    return BoxerDodgingFactorsDefines::GroupUid;
}

QString BoxerDodgingMultifactorDescriptor::name()
{
    return BoxerDodgingFactorsDefines::GroupName;
}

MultiFactor *BoxerDodgingMultifactorDescriptor::caclulate(const QString &testUid, const QString &probeUid, const QString &channelUid)
{
    Q_UNUSED(channelUid);
    return new BoxerDodgingMultifactor(testUid, probeUid);
}
