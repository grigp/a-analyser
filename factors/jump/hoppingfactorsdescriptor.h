#ifndef HOPPINGFACTORSDESCRIPTOR_H
#define HOPPINGFACTORSDESCRIPTOR_H

#include <QObject>

#include "basedefines.h"
#include "multifactordescriptor.h"

/*!
 * \brief Класс описателя показателей теста соскакивания на платформу The HoppingFactorsDescriptor class
 */
class HoppingFactorsDescriptor : public MultiFactorDescriptor
{
public:
    HoppingFactorsDescriptor(BaseDefines::TestLevel level);

    QString uid() override;
    QString name() override;

    MultiFactor* caclulate(const QString& testUid = "", const QString& probeUid = "", const QString& channelUid = "") override;
};

#endif // HOPPINGFACTORSDESCRIPTOR_H
