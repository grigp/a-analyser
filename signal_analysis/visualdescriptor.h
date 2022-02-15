#ifndef VISUALDESCRIPTOR_H
#define VISUALDESCRIPTOR_H

#include <QObject>

#include "visualdefines.h"

class Visual;

/*!
 * \brief Класс описателя визуализаторов The VisualDescriptor class
 */
class VisualDescriptor
{
public:

    VisualDescriptor(VisualDefines::Level level);
    virtual ~VisualDescriptor();

    virtual QString uid() = 0;
    virtual QString name() = 0;

    virtual Visual* getVisualWidget(QWidget *parent = nullptr,
                                    const QString& testUid = "", const QString& probeUid = "", const QString& channelUid = "") = 0;

    VisualDefines::Level level() {return m_level;}

private:
    VisualDefines::Level m_level {VisualDefines::vlNone};
};

#endif // VISUALDESCRIPTOR_H
