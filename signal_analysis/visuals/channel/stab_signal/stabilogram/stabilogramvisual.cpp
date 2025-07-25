#include "stabilogramvisual.h"

#include <QDebug>

#include "stabilogramvisualwidget.h"

StabilogramVisual::StabilogramVisual(BaseDefines::TestLevel level)
    : VisualDescriptor (level)
{
}

StabilogramVisual::~StabilogramVisual()
{
}

QString StabilogramVisual::uid()
{
    return VisualNames::UID_Stabilogram;
}

QString StabilogramVisual::name()
{
    return QCoreApplication::tr("Стабилограмма"); //VisualNames::Name_Stabilogram;
}

Visual *StabilogramVisual::getVisualWidget(QWidget *parent, const QString &testUid, const QString &probeUid, const QString &channelId,
                                           const QString& sectionNumber)
{
    Q_UNUSED(sectionNumber);
    return new StabilogramVisualWidget(this, testUid, probeUid, channelId, parent);
}

