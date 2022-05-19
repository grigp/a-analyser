#ifndef EVOLVENTAFACTORSDESCRIPTOR_H
#define EVOLVENTAFACTORSDESCRIPTOR_H

#include <QObject>

#include "basedefines.h"
#include "multifactordescriptor.h"

/*!
 * \brief Класс описателя показателей теста с "эвольвентой" The EvolventaFactorsDescriptor class
 */
class EvolventaFactorsDescriptor : public MultiFactorDescriptor
{
public:
    EvolventaFactorsDescriptor(BaseDefines::TestLevel level);

    QString uid() override;
    QString name() override;

    MultiFactor* caclulate(const QString& testUid = "", const QString& probeUid = "", const QString& channelUid = "") override;
};

#endif // EVOLVENTAFACTORSDESCRIPTOR_H
