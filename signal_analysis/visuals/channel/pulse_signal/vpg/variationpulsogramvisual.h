#ifndef VARIATIONPULSOGRAMVISUAL_H
#define VARIATIONPULSOGRAMVISUAL_H

#include <QObject>

#include "basedefines.h"
#include "visualdescriptor.h"

namespace VisualNames
{
static const QString UID_VPG = "vVPG";
static const QString Name_VPG = "Вариационная пульсограмма";
}


/*!
 * \brief Класс визуализатора вариационной пульсограммы The RitmogramVisual class
 */
class VariationPulsogramVisual : public VisualDescriptor
{
public:
    VariationPulsogramVisual(BaseDefines::TestLevel level);

    QString uid() override;
    QString name() override;

    Visual* getVisualWidget(QWidget *parent = nullptr,
                            const QString& testUid = "",
                            const QString& probeUid = "",
                            const QString& channelId = "",
                            const QString& sectionNumber = "") override;
};

#endif // VARIATIONPULSOGRAMVISUAL_H
