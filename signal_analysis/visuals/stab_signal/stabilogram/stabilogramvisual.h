#ifndef STABILOGRAMVISUAL_H
#define STABILOGRAMVISUAL_H

#include <QObject>

#include "visualdescriptor.h"

namespace VisualNames
{
static const QString UID_Stabilogram = "vStabilogram";

static const QString Name_Stabilogram = "Стабилограмма";


}

/*!
 * \brief Класс визуализатора стабилограммы The StabilogramVisual class
 */
class StabilogramVisual : public VisualDescriptor
{
public:
    StabilogramVisual(VisualDefines::Level level);

    QString uid() override;
    QString name() override;

    Visual* getVisualWidget(QWidget *parent = nullptr,
                            const QString& testUid = "", const QString& probeUid = "", const QString& channelUid = "") override;
};

#endif // STABILOGRAMVISUAL_H
