#ifndef STABILOGRAMVISUAL_H
#define STABILOGRAMVISUAL_H

#include <QObject>

#include "visuals.h"

/*!
 * \brief Класс визуализатора стабилограммы The StabilogramVisual class
 */
class StabilogramVisual : public ChannelVisual
{
public:
    StabilogramVisual(const QString& testUid, const QString& probeUid, const QString& channelUid);

    QString uid() override;
    QString name() override;

    bool isValid() override;
    Visual* getVisualWidget() override;
};

#endif // STABILOGRAMVISUAL_H
