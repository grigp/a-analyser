#ifndef STEPOFFSETFACTORSDESCRIPTOR_H
#define STEPOFFSETFACTORSDESCRIPTOR_H

#include <QObject>

#include "basedefines.h"
#include "multifactordescriptor.h"

/*!
 * \brief Класс описателя показателей теста "Ступени" The StepOffsetFactorsDescriptor class
 */
class StepOffsetFactorsDescriptor : public MultiFactorDescriptor
{
public:
    StepOffsetFactorsDescriptor(BaseDefines::TestLevel level);

    QString uid() override;
    QString name() override;

    /*!
     * \brief Возвращает true, если показатели можно рассчитать для теста с заданным uid
     */
    bool isValid(const QString &testUid, const QString &probeUid = "", const QString &channelId = "") override;

    MultiFactor* caclulate(const QString& testUid = "", const QString& probeUid = "", const QString& channelUid = "") override;
};

#endif // STEPOFFSETFACTORSDESCRIPTOR_H
