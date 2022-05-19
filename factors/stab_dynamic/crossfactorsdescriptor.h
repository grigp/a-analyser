#ifndef CROSSFACTORSDESCRIPTOR_H
#define CROSSFACTORSDESCRIPTOR_H

#include <QObject>

#include "basedefines.h"
#include "multifactordescriptor.h"

/*!
 * \brief Класс описателя показателей теста на устойчивость The CrossFactorsDescriptor class
 */
class CrossFactorsDescriptor : public MultiFactorDescriptor
{
public:
    CrossFactorsDescriptor(BaseDefines::TestLevel level);

    QString uid() override;
    QString name() override;

    MultiFactor* caclulate(const QString& testUid = "", const QString& probeUid = "", const QString& channelUid = "") override;
};

#endif // CROSSFACTORSDESCRIPTOR_H
