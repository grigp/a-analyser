#ifndef SPECTRSTABVISUAL_H
#define SPECTRSTABVISUAL_H

#include <QObject>

#include "basedefines.h"
#include "visualdescriptor.h"

namespace VisualNames
{
static const QString UID_SpectrStab = "vSpectrStab";
static const QString Name_SpectrStab = "Спектральный анализ";
}

/*!
 * \brief Класс визуализатора спектрального анализа стабилограммы The SpectrStabVisual class
 */
class SpectrStabVisual : public VisualDescriptor
{
public:
    SpectrStabVisual(BaseDefines::TestLevel level);
    ~SpectrStabVisual() override;

    QString uid() override;
    QString name() override;

    Visual* getVisualWidget(QWidget *parent = nullptr,
                            const QString& testUid = "",
                            const QString& probeUid = "",
                            const QString& channelId = "",
                            const QString& sectionNumber = "") override;
};

#endif // SPECTRSTABVISUAL_H
