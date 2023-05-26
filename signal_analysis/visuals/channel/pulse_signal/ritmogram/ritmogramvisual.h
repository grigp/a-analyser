#ifndef RITMOGRAMVISUAL_H
#define RITMOGRAMVISUAL_H

#include <QObject>

#include "basedefines.h"
#include "visualdescriptor.h"

namespace VisualNames
{
static const QString UID_Ritmogram = "vRitmogram";
static const QString Name_Ritmogram = "Ритмограмма";
}


/*!
 * \brief Класс визуализатора сигнала ритмограммы The RitmogramVisual class
 */
class RitmogramVisual : public VisualDescriptor
{
public:
    RitmogramVisual(BaseDefines::TestLevel level);

    QString uid() override;
    QString name() override;

    Visual* getVisualWidget(QWidget *parent = nullptr,
                            const QString& testUid = "",
                            const QString& probeUid = "",
                            const QString& channelId = "",
                            const QString& sectionNumber = "") override;
};

#endif // RITMOGRAMVISUAL_H
