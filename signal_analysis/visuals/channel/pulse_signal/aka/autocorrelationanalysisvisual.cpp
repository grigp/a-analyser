#include "autocorrelationanalysisvisual.h"

#include "autocorrelationanalysisvisualwidget.h"

AutoCorrelationAnalysisVisual::AutoCorrelationAnalysisVisual(BaseDefines::TestLevel level)
    : VisualDescriptor (level)
{

}

QString AutoCorrelationAnalysisVisual::uid()
{
    return VisualNames::UID_AKA;
}

QString AutoCorrelationAnalysisVisual::name()
{
    return QCoreApplication::tr("Автокорреляционный анализ"); //VisualNames::Name_AKA;
}

Visual *AutoCorrelationAnalysisVisual::getVisualWidget(QWidget *parent,
                                                       const QString &testUid,
                                                       const QString &probeUid,
                                                       const QString &channelId,
                                                       const QString &sectionNumber)
{
    Q_UNUSED(sectionNumber);
    return new AutoCorrelationAnalysisVisualWidget(this, testUid, probeUid, channelId, parent);
}
