#ifndef MULTIFACTORDESCRIPTOR_H
#define MULTIFACTORDESCRIPTOR_H

#include <QObject>

#include "basedefines.h"

class MultiFactor;

/*!
 * \brief Класс описателя групп показателей The MultiFactorDescriptor class
 */
class MultiFactorDescriptor
{
public:
    MultiFactorDescriptor(BaseDefines::TestLevel level);
    virtual ~MultiFactorDescriptor();

    virtual QString uid() = 0;
    virtual QString name() = 0;

    virtual MultiFactor* caclulate(const QString& testUid = "", const QString& probeUid = "", const QString& channelUid = "") = 0;

    BaseDefines::TestLevel level() {return m_level;}

private:
    BaseDefines::TestLevel m_level {BaseDefines::tlNone};

};



#endif // MULTIFACTORDESCRIPTOR_H