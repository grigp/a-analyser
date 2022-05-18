#ifndef VECTORFACTORSDESCRIPTOR_H
#define VECTORFACTORSDESCRIPTOR_H

#include <QObject>

#include "multifactordescriptor.h"

/*!
 * \brief Класс описателя векторных показателей The VectorFactorsDescriptor class
 */
class VectorFactorsDescriptor : public MultiFactorDescriptor
{
public:
    VectorFactorsDescriptor(BaseDefines::TestLevel level);

    QString uid() override;
    QString name() override;

    MultiFactor* caclulate(const QString& testUid = "", const QString& probeUid = "", const QString& channelUid = "") override;
};

#endif // VECTORFACTORSDESCRIPTOR_H
