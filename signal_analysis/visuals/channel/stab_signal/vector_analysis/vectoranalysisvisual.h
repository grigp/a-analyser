#ifndef VECTORANALYSISVISUAL_H
#define VECTORANALYSISVISUAL_H

#include <QObject>

#include "basedefines.h"
#include "visualdescriptor.h"

namespace VisualNames
{
static const QString UID_VectorAnalysis = "vVectorAnalysis";
static const QString Name_VectorAnalysis = "Анализ векторов";
}

/*!
 * \brief Класс визуализатора анализа векторов The VectorAnalysisVisual class
 */
class VectorAnalysisVisual : public VisualDescriptor
{
public:
    VectorAnalysisVisual(BaseDefines::TestLevel level);
    ~VectorAnalysisVisual() override;

    QString uid() override;
    QString name() override;

    Visual* getVisualWidget(QWidget *parent = nullptr,
                            const QString& testUid = "", const QString& probeUid = "", const QString& channelId = "") override;
};

#endif // VECTORANALYSISVISUAL_H
