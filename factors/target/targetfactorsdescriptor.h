#ifndef TARGETFACTORSDESCRIPTOR_H
#define TARGETFACTORSDESCRIPTOR_H

#include <QObject>

#include "multifactordescriptor.h"

/*!
 * \brief Класс описателя показателей мишени The TargetFactorsDescriptor class
 */
class TargetFactorsDescriptor : public MultiFactorDescriptor
{
public:
    TargetFactorsDescriptor(BaseDefines::TestLevel level);

    QString uid() override;
    QString name() override;

    MultiFactor* caclulate(const QString& testUid = "", const QString& probeUid = "", const QString& channelUid = "") override;
};

#endif // TARGETFACTORSDESCRIPTOR_H
