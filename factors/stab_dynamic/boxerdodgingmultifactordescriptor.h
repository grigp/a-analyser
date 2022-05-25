#ifndef BOXERDODGINGMULTIFACTORDESCRIPTOR_H
#define BOXERDODGINGMULTIFACTORDESCRIPTOR_H

#include <QObject>

#include "basedefines.h"
#include "multifactordescriptor.h"

/*!
 * \brief Класс описателя показателей тренажера с уклонением для боксеров The BoxerDodgingMultifactorDescriptor class
 */
class BoxerDodgingMultifactorDescriptor : public MultiFactorDescriptor
{
public:
    BoxerDodgingMultifactorDescriptor(BaseDefines::TestLevel level);

    QString uid() override;
    QString name() override;

    /*!
     * \brief Возвращает true, если показатели можно рассчитать для теста с заданным uid
     */
    bool isValid(const QString &testUid, const QString &probeUid = "", const QString &channelId = "") override;

    MultiFactor* caclulate(const QString& testUid = "", const QString& probeUid = "", const QString& channelUid = "") override;
};

#endif // BOXERDODGINGMULTIFACTORDESCRIPTOR_H
