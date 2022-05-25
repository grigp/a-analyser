#ifndef IDSFACTORSDESCRIPTOR_H
#define IDSFACTORSDESCRIPTOR_H

#include <QObject>

#include "multifactordescriptor.h"

/*!
 * \brief Класс описателя показателей ИДС The IDSFactorsDescriptor class
 */
class IDSFactorsDescriptor : public MultiFactorDescriptor
{
public:
    IDSFactorsDescriptor(BaseDefines::TestLevel level);

    QString uid() override;
    QString name() override;

    /*!
     * \brief Возвращает true, если показатели можно рассчитать для теста с заданным uid
     */
    bool isValid(const QString &testUid, const QString &probeUid = "", const QString &channelId = "") override;

    MultiFactor* caclulate(const QString& testUid = "", const QString& probeUid = "", const QString& channelUid = "") override;
};

#endif // IDSFACTORSDESCRIPTOR_H
