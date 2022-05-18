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

    MultiFactor* caclulate(const QString& testUid = "", const QString& probeUid = "", const QString& channelUid = "") override;
};

#endif // TRENRESULTFACTORSDESCRIPTOR_H
