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

bool BoxerDodgingMultifactorDescriptor::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    Q_UNUSED(channelId);
    return BoxerDodgingMultifactor::isValid(testUid, probeUid);
}

MultiFactor *BoxerDodgingMultifactorDescriptor::caclulate(const QString &testUid,
                                                          const QString &probeUid,
                                                          const QString &channelUid,
                                                          const int begin,
                                                          const int end)
{
    Q_UNUSED(channelUid);
    Q_UNUSED(begin);
    Q_UNUSED(end);
    return new BoxerDodgingMultifactor(testUid, probeUid);
}
