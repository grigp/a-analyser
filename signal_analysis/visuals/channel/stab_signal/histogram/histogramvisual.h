#ifndef HISTOGRAMVISUAL_H
#define HISTOGRAMVISUAL_H

#include <QObject>

#include "basedefines.h"
#include "visualdescriptor.h"

namespace VisualNames
{
static const QString UID_Histogram = "vHistogram";
static const QString Name_Histogram = "Гистограмма";
}

/*!
 * \brief Класс визуализатора гистограммы The HistogramVisual class
 */
class HistogramVisual : public VisualDescriptor
{
public:
    HistogramVisual(BaseDefines::TestLevel level);
    ~HistogramVisual() override;

    QString uid() override;
    QString name() override;

    Visual* getVisualWidget(QWidget *parent = nullptr,
                            const QString& testUid = "", const QString& probeUid = "", const QString& channelUid = "") override;
};

#endif // HISTOGRAMVISUAL_H
