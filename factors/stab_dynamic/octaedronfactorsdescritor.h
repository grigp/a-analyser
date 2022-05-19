#ifndef OCTAEDRONFACTORSDESCRITOR_H
#define OCTAEDRONFACTORSDESCRITOR_H

#include <QObject>

#include "basedefines.h"
#include "multifactordescriptor.h"

/*!
 * \brief Класс описателя показателей тренажера "Октаэдр" The OctaedronFactorsDescrition class
 */
class OctaedronFactorsDescritior : public MultiFactorDescriptor
{
public:
    OctaedronFactorsDescritior(BaseDefines::TestLevel level);

    QString uid() override;
    QString name() override;

    MultiFactor* caclulate(const QString& testUid = "", const QString& probeUid = "", const QString& channelUid = "") override;
};

#endif // OCTAEDRONFACTORSDESCRITOR_H
