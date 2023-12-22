#ifndef GENERALCOORDFACTORSDESCRIPTOR_H
#define GENERALCOORDFACTORSDESCRIPTOR_H

#include <QObject>

#include "basedefines.h"
#include "multifactordescriptor.h"

/*!
 * \brief Класс описателя показателей оценки координационных способностей The GeneralCoordFactorsDescriptor class
 * Не используется так, как группа показателей рассчитывается только напрямую из визуализатора
 */
class GeneralCoordFactorsDescriptor : public MultiFactorDescriptor
{
public:
    GeneralCoordFactorsDescriptor(BaseDefines::TestLevel level);

    QString uid() override;
    QString name() override;

    /*!
     * \brief Возвращает true, если показатели можно рассчитать для теста с заданным uid
     */
    bool isValid(const QString &testUid = "", const QString &probeUid = "", const QString &channelId = "") override;

    MultiFactor* caclulate(const QString& testUid = "",
                           const QString& probeUid = "",
                           const QString& channelUid = "",
                           const int begin = -1,
                           const int end  = -1) override;
};

#endif // GENERALCOORDFACTORSDESCRIPTOR_H
