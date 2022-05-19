#ifndef TEPPINGTESTFACTORSDESCRIPTOR_H
#define TEPPINGTESTFACTORSDESCRIPTOR_H

#include <QObject>

#include "basedefines.h"
#include "multifactordescriptor.h"

/*!
 * \brief Класс описателя показателей теппинг теста The TeppingTestFactorsDescriptor class
 */
class TeppingTestFactorsDescriptor : public MultiFactorDescriptor
{
public:
    TeppingTestFactorsDescriptor(BaseDefines::TestLevel level);

    QString uid() override;
    QString name() override;

    MultiFactor* caclulate(const QString& testUid = "", const QString& probeUid = "", const QString& channelUid = "") override;

};

#endif // TEPPINGTESTFACTORSDESCRIPTOR_H
