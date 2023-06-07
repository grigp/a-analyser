#ifndef SPECTRPULSEVISUAL_H
#define SPECTRPULSEVISUAL_H

#include <QObject>


#include "basedefines.h"
#include "visualdescriptor.h"

namespace VisualNames
{
static const QString UID_SpectrPulse = "vSpectrPulse";
static const QString Name_SpectrPulse = "Спектральный анализ";
}


/*!
 * \brief Класс визуализатора спектрального анализа пульса The SpectrPulseVisual class
 */
class SpectrPulseVisual : public VisualDescriptor
{
public:
    SpectrPulseVisual(BaseDefines::TestLevel level);

    QString uid() override;
    QString name() override;

    Visual* getVisualWidget(QWidget *parent = nullptr,
                            const QString& testUid = "",
                            const QString& probeUid = "",
                            const QString& channelId = "",
                            const QString& sectionNumber = "") override;
};

#endif // SPECTRPULSEVISUAL_H
