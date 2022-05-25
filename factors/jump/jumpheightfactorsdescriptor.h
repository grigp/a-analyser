#ifndef JUMPHEIGHTFACTORSDESCRIPTOR_H
#define JUMPHEIGHTFACTORSDESCRIPTOR_H

#include <QObject>

#include "basedefines.h"
#include "multifactordescriptor.h"

/*!
 * \brief Класс описателя показателей теста высоты прыжков The JumpHeightFactorsDescriptor class
 */
class JumpHeightFactorsDescriptor : public MultiFactorDescriptor
{
public:
    JumpHeightFactorsDescriptor(BaseDefines::TestLevel level);

    QString uid() override;
    QString name() override;

    /*!
     * \brief Возвращает true, если показатели можно рассчитать для теста с заданным uid
     */
    bool isValid(const QString &testUid, const QString &probeUid = "", const QString &channelId = "") override;

    MultiFactor* caclulate(const QString& testUid = "", const QString& probeUid = "", const QString& channelUid = "") override;
};

#endif // JUMPHEIGHTFACTORSDESCRIPTOR_H
