#ifndef WEIGHTPLATEGRAPHVISUAL_H
#define WEIGHTPLATEGRAPHVISUAL_H

#include <QObject>

#include "visualdescriptor.h"

namespace VisualNames
{
static const QString UID_WeightPlateGraph = "vWeightPlateGraph";
static const QString Name_WeightPlateGraph = "Анализ динамики веса";
}

/*!
 * \brief Класс визуализатора графика динамики веса The WeightPlateGraphVisual class
 */
class WeightPlateGraphVisual : public VisualDescriptor
{
public:
    WeightPlateGraphVisual(VisualDefines::Level level);
    ~WeightPlateGraphVisual() override;

    QString uid() override;
    QString name() override;

    Visual* getVisualWidget(QWidget *parent = nullptr,
                            const QString& testUid = "", const QString& probeUid = "", const QString& channelUid = "") override;

};

#endif // WEIGHTPLATEGRAPHVISUAL_H