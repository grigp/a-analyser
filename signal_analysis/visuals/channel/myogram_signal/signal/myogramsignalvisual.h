#ifndef MYOGRAMSIGNALVISUAL_H
#define MYOGRAMSIGNALVISUAL_H

#include <QObject>

#include "visualdescriptor.h"


namespace VisualNames
{
static const QString UID_MyogramSignal = "vMyogramSignal";
static const QString Name_MyogramSignal = "Сигнал";
}


/*!
 * \brief Класс визуализатора сигнала миограммы The MyogramSignalVisual class
 */
class MyogramSignalVisual : public VisualDescriptor
{
public:
    MyogramSignalVisual(VisualDefines::Level level);
    ~MyogramSignalVisual() override;

    QString uid() override;
    QString name() override;

    Visual* getVisualWidget(QWidget *parent = nullptr,
                            const QString& testUid = "", const QString& probeUid = "", const QString& channelUid = "") override;
};

#endif // MYOGRAMSIGNALVISUAL_H
