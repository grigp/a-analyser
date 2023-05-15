#ifndef VECTORFACTORSDESCRIPTOR_H
#define VECTORFACTORSDESCRIPTOR_H

#include <QObject>

#include "multifactordescriptor.h"

/*!
 * \brief Класс описателя векторных показателей The VectorFactorsDescriptor class
 */
class VectorFactorsDescriptor : public MultiFactorDescriptor
{
public:
    VectorFactorsDescriptor(BaseDefines::TestLevel level);

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

#endif // VECTORFACTORSDESCRIPTOR_H
