#ifndef TRIANGLEFACTORSDESCRIPTOR_H
#define TRIANGLEFACTORSDESCRIPTOR_H

#include <QObject>

#include "basedefines.h"
#include "multifactordescriptor.h"

/*!
 * \brief Класс описателя показателей теста "Треугольник" The TriangleFactorsDescriptor class
 */
class TriangleFactorsDescriptor : public MultiFactorDescriptor
{
public:
    TriangleFactorsDescriptor(BaseDefines::TestLevel level);

    QString uid() override;
    QString name() override;

    /*!
     * \brief Возвращает true, если показатели можно рассчитать для теста с заданным uid
     */
    bool isValid(const QString &testUid, const QString &probeUid = "", const QString &channelId = "") override;

    MultiFactor* caclulate(const QString& testUid = "", const QString& probeUid = "", const QString& channelUid = "") override;
};

#endif // TRIANGLEFACTORSDESCRIPTOR_H
