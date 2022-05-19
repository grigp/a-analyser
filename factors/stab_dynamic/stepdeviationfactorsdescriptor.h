#ifndef STEPDEVIATIONFACTORSDESCRIPTOR_H
#define STEPDEVIATIONFACTORSDESCRIPTOR_H

#include <QObject>

#include "basedefines.h"
#include "multifactordescriptor.h"

/*!
 * \brief Класс описателя показателей теста прироста The StepDeviationFactorsDescriptor class
 */
class StepDeviationFactorsDescriptor : public MultiFactorDescriptor
{
public:
    StepDeviationFactorsDescriptor(BaseDefines::TestLevel level);

    QString uid() override;
    QString name() override;

    MultiFactor* caclulate(const QString& testUid = "", const QString& probeUid = "", const QString& channelUid = "") override;
};

#endif // STEPDEVIATIONFACTORSDESCRIPTOR_H
