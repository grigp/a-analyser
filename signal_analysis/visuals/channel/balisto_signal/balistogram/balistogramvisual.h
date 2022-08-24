#ifndef BALISTOGRAMVISUAL_H
#define BALISTOGRAMVISUAL_H

#include <QObject>

#include "basedefines.h"
#include "visualdescriptor.h"

namespace VisualNames
{
static const QString UID_Balistogram = "vBalistogram";
static const QString Name_Balistogram = "Балистограмма";
}

/*!
 * \brief Класс визуализатора балистограммы The BalistogramVisual class
 */
class BalistogramVisual : public VisualDescriptor
{
public:
    BalistogramVisual(BaseDefines::TestLevel level);
    ~BalistogramVisual() override;

    QString uid() override;
    QString name() override;

    Visual* getVisualWidget(QWidget *parent = nullptr,
                            const QString& testUid = "", const QString& probeUid = "", const QString& channelId = "") override;
};

#endif // BALISTOGRAMVISUAL_H
