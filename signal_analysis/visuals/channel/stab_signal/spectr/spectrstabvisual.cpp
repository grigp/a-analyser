#include "spectrstabvisual.h"

#include "spectrstabvisualwidget.h"

SpectrStabVisual::SpectrStabVisual(BaseDefines::TestLevel level)
    : VisualDescriptor (level)
{

}

SpectrStabVisual::~SpectrStabVisual()
{

}

QString SpectrStabVisual::uid()
{
    return VisualNames::UID_SpectrStab;
}

QString SpectrStabVisual::name()
{
    return QCoreApplication::tr("Спектральный анализ"); //VisualNames::Name_SpectrStab;
}

Visual *SpectrStabVisual::getVisualWidget(QWidget *parent, const QString &testUid, const QString &probeUid, const QString &channelId,
                                          const QString& sectionNumber)
{
    Q_UNUSED(sectionNumber);
    return new SpectrStabVisualWidget(this, testUid, probeUid, channelId, parent);
}
