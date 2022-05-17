#ifndef DYNAMOSIGNALVISUAL_H
#define DYNAMOSIGNALVISUAL_H

#include <QObject>

#include "basedefines.h"
#include "visualdescriptor.h"

namespace VisualNames
{
static const QString UID_DynamoSignal = "vDynamoSignal";
static const QString Name_DynamoSignal = "Сигнал";
}


/*!
 * \brief Класс визуализатора сигнала силомера The DynamoSignalVisual class
 */
class DynamoSignalVisual : public VisualDescriptor
{
public:
    DynamoSignalVisual(BaseDefines::TestLevel level);
    ~DynamoSignalVisual() override;

    QString uid() override;
    QString name() override;

    Visual* getVisualWidget(QWidget *parent = nullptr,
                            const QString& testUid = "", const QString& probeUid = "", const QString& channelUid = "") override;
};

#endif // DYNAMOSIGNALVISUAL_H
