#ifndef BILATERALVISUAL_H
#define BILATERALVISUAL_H

#include <QObject>

#include "visualdescriptor.h"

namespace VisualNames
{
static const QString UID_Bilateral = "vBilateral";
static const QString Name_Bilateral = "Билатеральный режим";
}


class BilateralVisual : public VisualDescriptor
{
public:
    BilateralVisual(VisualDefines::Level level);

    QString uid() override;
    QString name() override;

    Visual* getVisualWidget(QWidget *parent = nullptr,
                            const QString& testUid = "", const QString& probeUid = "", const QString& channelUid = "") override;
};

#endif // BILATERALVISUAL_H
