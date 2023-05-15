#ifndef PULSEFACTORSDESCRIPTOR_H
#define PULSEFACTORSDESCRIPTOR_H

#include <QObject>

#include "basedefines.h"
#include "multifactordescriptor.h"


/*!
 * \brief Класс описателя показателей ритмограммы The PulseFactorsDescriptor class
 */
class PulseFactorsDescriptor : public MultiFactorDescriptor
{
public:
    PulseFactorsDescriptor(BaseDefines::TestLevel level);

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

#endif // PULSEFACTORSDESCRIPTOR_H
