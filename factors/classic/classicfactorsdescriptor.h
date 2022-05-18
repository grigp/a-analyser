#ifndef CLASSICFACTORSDESCRIPTOR_H
#define CLASSICFACTORSDESCRIPTOR_H

#include <QObject>

#include "basedefines.h"
#include "multifactordescriptor.h"

/*!
 * \brief Класс описателя классических показателей The ClassicFactorsDescriptor class
 */
class ClassicFactorsDescriptor : public MultiFactorDescriptor
{
public:
    ClassicFactorsDescriptor(BaseDefines::TestLevel level);

    QString uid() override;
    QString name() override;

    MultiFactor* caclulate(const QString& testUid = "", const QString& probeUid = "", const QString& channelUid = "") override;
};

#endif // CLASSICFACTORSDESCRIPTOR_H
