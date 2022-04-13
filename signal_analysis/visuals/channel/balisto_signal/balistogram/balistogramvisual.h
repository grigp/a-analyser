#ifndef BALISTOGRAMVISUAL_H
#define BALISTOGRAMVISUAL_H

#include <QObject>

#include "visualdescriptor.h"

namespace VisualNames
{
static const QString UID_Balistogram = "vBalistogram";
static const QString Name_Balistogram = "Балистограмма";
}


class BalistogramVisual : public VisualDescriptor
{
public:
    BalistogramVisual(VisualDefines::Level level);
    ~BalistogramVisual() override;

    QString uid() override;
    QString name() override;

    Visual* getVisualWidget(QWidget *parent = nullptr,
                            const QString& testUid = "", const QString& probeUid = "", const QString& channelUid = "") override;
};

#endif // BALISTOGRAMVISUAL_H
