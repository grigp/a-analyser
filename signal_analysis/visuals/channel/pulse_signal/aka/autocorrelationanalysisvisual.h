#ifndef AUTOCORRELATIONANALYSISVISUAL_H
#define AUTOCORRELATIONANALYSISVISUAL_H

#include <QObject>

#include "basedefines.h"
#include "visualdescriptor.h"

namespace VisualNames
{
static const QString UID_AKA = "vAKA";
static const QString Name_AKA = "Автокорреляционный анализ";
}


/*!
 * \brief Класс визуализатора автокорреляционного анализа The AutoCorrelationAnalysisVisual class
 */
class AutoCorrelationAnalysisVisual : public VisualDescriptor
{
public:
    AutoCorrelationAnalysisVisual(BaseDefines::TestLevel level);

    QString uid() override;
    QString name() override;

    Visual* getVisualWidget(QWidget *parent = nullptr,
                            const QString& testUid = "",
                            const QString& probeUid = "",
                            const QString& channelId = "",
                            const QString& sectionNumber = "") override;
};

#endif // AUTOCORRELATIONANALYSISVISUAL_H
