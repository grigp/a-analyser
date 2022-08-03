#ifndef SPECTRSTABFACTORDESCRIPTOR_H
#define SPECTRSTABFACTORDESCRIPTOR_H

#include <QObject>

#include "multifactordescriptor.h"

/*!
 * \brief Класс описателя показателей спектра стабилограммы The SpectrStabFactorsDescriptor class
 */
class SpectrStabFactorsDescriptor : public MultiFactorDescriptor
{
public:
    SpectrStabFactorsDescriptor(BaseDefines::TestLevel level);

    QString uid() override;
    QString name() override;

    /*!
     * \brief Возвращает true, если показатели можно рассчитать для теста с заданным uid
     */
    bool isValid(const QString &testUid, const QString &probeUid = "", const QString &channelId = "") override;

    MultiFactor* caclulate(const QString& testUid = "", const QString& probeUid = "", const QString& channelUid = "") override;
};

#endif // SPECTRSTABFACTORDESCRIPTOR_H
