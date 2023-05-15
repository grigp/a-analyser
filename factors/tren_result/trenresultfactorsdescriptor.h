#ifndef TRENRESULTFACTORSDESCRIPTOR_H
#define TRENRESULTFACTORSDESCRIPTOR_H

#include <QObject>

#include "multifactordescriptor.h"

/*!
 * \brief Класс описателя показателей тренажеров The TrenResultFactorsDescriptor class
 */
class TrenResultFactorsDescriptor : public MultiFactorDescriptor
{
public:
    TrenResultFactorsDescriptor(BaseDefines::TestLevel level);

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

#endif // TRENRESULTFACTORSDESCRIPTOR_H
