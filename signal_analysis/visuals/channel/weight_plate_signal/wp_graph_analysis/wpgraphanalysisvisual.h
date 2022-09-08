#ifndef WPGRAPHANALYSISVISUAL_H
#define WPGRAPHANALYSISVISUAL_H

#include <QObject>

#include "basedefines.h"
#include "visualdescriptor.h"

namespace VisualNames
{
static const QString UID_WPGraphAnalysis = "vWPGraphAnalysis";
static const QString Name_WPGraphAnalysis = "Анализ сигналов";
}

/*!
 * \brief Класс визуализатора анализа графиков динамики веса The WPGraphAnalysisVisual class
 */
class WPGraphAnalysisVisual : public VisualDescriptor
{
public:
    WPGraphAnalysisVisual(BaseDefines::TestLevel level);
    ~WPGraphAnalysisVisual() override;

    QString uid() override;
    QString name() override;

    Visual* getVisualWidget(QWidget *parent = nullptr,
                            const QString& testUid = "",
                            const QString& probeUid = "",
                            const QString& channelId = "",
                            const QString& sectionNumber = "") override;
};

#endif // WPGRAPHANALYSISVISUAL_H
