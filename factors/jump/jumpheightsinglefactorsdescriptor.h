#ifndef JUMPHEIGHTSINGLEFACTORSDESCRIPTOR_H
#define JUMPHEIGHTSINGLEFACTORSDESCRIPTOR_H

#include <QObject>

#include "basedefines.h"
#include "multifactordescriptor.h"

/*!
 * \brief Класс описателя показателей теста высоты одиночного прыжка The JumpHeightSingleFactorsDescriptor class
 */
class JumpHeightSingleFactorsDescriptor : public MultiFactorDescriptor
{
public:
    JumpHeightSingleFactorsDescriptor(BaseDefines::TestLevel level);

    QString uid() override;
    QString name() override;

    MultiFactor* caclulate(const QString& testUid = "", const QString& probeUid = "", const QString& channelUid = "") override;
};

#endif // JUMPHEIGHTSINGLEFACTORSDESCRIPTOR_H
