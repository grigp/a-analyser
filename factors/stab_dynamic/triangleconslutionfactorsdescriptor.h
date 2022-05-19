#ifndef TRIANGLECONSLUTIONFACTORSDESCRIPTOR_H
#define TRIANGLECONSLUTIONFACTORSDESCRIPTOR_H

#include <QObject>

#include "basedefines.h"
#include "multifactordescriptor.h"

/*!
 * \brief Класс описателя показателей заключения по тесту "Треугольник" The TriangleConslutionFactorsDescriptor class
 */
class TriangleConslutionFactorsDescriptor : public MultiFactorDescriptor
{
public:
    TriangleConslutionFactorsDescriptor(BaseDefines::TestLevel level);

    QString uid() override;
    QString name() override;

    MultiFactor* caclulate(const QString& testUid = "", const QString& probeUid = "", const QString& channelUid = "") override;
};

#endif // TRIANGLECONSLUTIONFACTORSDESCRIPTOR_H
