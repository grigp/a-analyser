#ifndef RATIOPROBESFACTORSDESCRIPTOR_H
#define RATIOPROBESFACTORSDESCRIPTOR_H

#include <QObject>

#include "basedefines.h"
#include "multifactordescriptor.h"

/*!
 * \brief Класс описателя показателей отношений между пробами The RatioProbesFactorsDescriptor class
 */
class RatioProbesFactorsDescriptor : public MultiFactorDescriptor
{
public:
    RatioProbesFactorsDescriptor(BaseDefines::TestLevel level);

    QString uid() override;
    QString name() override;

    MultiFactor* caclulate(const QString& testUid = "", const QString& probeUid = "", const QString& channelUid = "") override;
};

#endif // RATIOPROBESFACTORSDESCRIPTOR_H
