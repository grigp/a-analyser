#ifndef VISUALDESCRIPTOR_H
#define VISUALDESCRIPTOR_H

#include <QObject>

#include "basedefines.h"

class Visual;

/*!
 * \brief Класс описателя визуализаторов The VisualDescriptor class
 */
class VisualDescriptor
{
public:

    VisualDescriptor(BaseDefines::TestLevel level);
    virtual ~VisualDescriptor();

    virtual QString uid() = 0;
    virtual QString name() = 0;

    virtual Visual* getVisualWidget(QWidget *parent = nullptr,
                                    const QString& testUid = "", const QString& probeUid = "", const QString& channelId = "") = 0;

    BaseDefines::TestLevel level() {return m_level;}

private:
    BaseDefines::TestLevel m_level {BaseDefines::tlNone};
};

#endif // VISUALDESCRIPTOR_H
