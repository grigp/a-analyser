#ifndef DROPTESTFACTORSDESCRIPTOR_H
#define DROPTESTFACTORSDESCRIPTOR_H

#include <QObject>

#include "basedefines.h"
#include "multifactordescriptor.h"

/*!
 * \brief Класс описателя показателей дроп теста The DropTestFactorsDescriptor class
 */
class DropTestFactorsDescriptor : public MultiFactorDescriptor
{
public:
    DropTestFactorsDescriptor(BaseDefines::TestLevel level);

    QString uid() override;
    QString name() override;

    /*!
     * \brief Возвращает true, если показатели можно рассчитать для теста с заданным uid
     */
    bool isValid(const QString &testUid, const QString &probeUid = "", const QString &channelId = "") override;

    MultiFactor* caclulate(const QString& testUid = "", const QString& probeUid = "", const QString& channelUid = "") override;
};

#endif // DROPTESTFACTORSDESCRIPTOR_H
