#ifndef VECTORANALYSISVISUAL_H
#define VECTORANALYSISVISUAL_H

#include <QObject>

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
    VectorAnalysisVisual(VisualDefines::Level level);
    ~VectorAnalysisVisual() override;

    QString uid() override;
    QString name() override;

    Visual* getVisualWidget(QWidget *parent = nullptr,
                            const QString& testUid = "", const QString& probeUid = "", const QString& channelUid = "") override;
};

#endif // VECTORANALYSISVISUAL_H
