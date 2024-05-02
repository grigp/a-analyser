#include "rehabreportppvisual.h"

#include "rehabreportppvisualwidget.h"

RehabReportPPVisual::RehabReportPPVisual()
{

}

RehabReportPPVisual::~RehabReportPPVisual()
{

}

QString RehabReportPPVisual::uid()
{
    return PPVisualNames::UID_RehabReport;
}

QString RehabReportPPVisual::name()
{
    return PPVisualNames::Name_RehabReport;
}

PPVisual *RehabReportPPVisual::getVisualWidget(QJsonObject &objPP, QWidget *parent)
{
    return new RehabReportPPVisualWidget(this, objPP, parent);
}
