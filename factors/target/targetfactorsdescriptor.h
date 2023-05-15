#ifndef TARGETFACTORSDESCRIPTOR_H
#define TARGETFACTORSDESCRIPTOR_H

#include <QObject>

#include "multifactordescriptor.h"

/*!
 * \brief Класс описателя показателей мишени The TargetFactorsDescriptor class
 */
class TargetFactorsDescriptor : public MultiFactorDescriptor
{
public:
    TargetFactorsDescriptor(BaseDefines::TestLevel level);

    QString uid() override;
    QString name() override;

    /*!
     * \brief Возвращает true, если показатели можно рассчитать для теста с заданным uid
     */
    bool isValid(const QString &testUid, const QString &probeUid = "", const QString &channelId = "") override;

    MultiFactor* caclulate(const QString& testUid = "",
                           const QString& probeUid = "",
                           const QString& channelUid = "",
                           const int begin = -1,
                           const int end  = -1) override;
};

#endif // TARGETFACTORSDESCRIPTOR_H
