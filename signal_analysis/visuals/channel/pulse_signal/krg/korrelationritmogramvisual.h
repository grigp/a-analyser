#ifndef KORRELATIONRITMOGRAMVISUAL_H
#define KORRELATIONRITMOGRAMVISUAL_H

#include <QObject>

#include "basedefines.h"
#include "visualdescriptor.h"

namespace VisualNames
{
static const QString UID_KRG = "vKRG";
static const QString Name_KRG = "Корреляционная ритмограмма";
}


/*!
 * \brief Класс визуализатора сигнала корреляционной ритмограммы The KorrelationRitmogramVisual class
 */
class KorrelationRitmogramVisual : public VisualDescriptor
{
public:
    KorrelationRitmogramVisual(BaseDefines::TestLevel level);

    QString uid() override;
    QString name() override;

    Visual* getVisualWidget(QWidget *parent = nullptr,
                            const QString& testUid = "",
                            const QString& probeUid = "",
                            const QString& channelId = "",
                            const QString& sectionNumber = "") override;
};

#endif // KORRELATIONRITMOGRAMVISUAL_H
