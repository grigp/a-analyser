#ifndef SKGVISUAL_H
#define SKGVISUAL_H

#include <QObject>

#include "basedefines.h"
#include "visualdescriptor.h"

namespace VisualNames
{
static const QString UID_SKG = "vSKG";
static const QString Name_SKG = "Статокинезиграмма";
}

/*!
 * \brief Класс визуализатора стабилограммы The SKGVisual class
 */
class SKGVisual : public VisualDescriptor
{
public:
    SKGVisual(BaseDefines::TestLevel level);

    QString uid() override;
    QString name() override;

    Visual* getVisualWidget(QWidget *parent = nullptr,
                            const QString& testUid = "", const QString& probeUid = "", const QString& channelId = "") override;
};

#endif // SKGVISUAL_H
