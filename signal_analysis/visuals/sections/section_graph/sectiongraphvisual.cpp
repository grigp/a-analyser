#include "sectiongraphvisual.h"

#include "sectiongraphvisualwidget.h"

SectionGraphVisual::SectionGraphVisual(BaseDefines::TestLevel level)
    : VisualDescriptor (level)
{

}

SectionGraphVisual::~SectionGraphVisual()
{

}

QString SectionGraphVisual::uid()
{
    return VisualNames::UID_SectionGraph;
}

QString SectionGraphVisual::name()
{
    return VisualNames::Name_SectionGraph;
}

Visual *SectionGraphVisual::getVisualWidget(QWidget *parent,
                                            const QString &testUid,
                                            const QString &probeUid,
                                            const QString &channelId,
                                            const QString &sectionNumber)
{
    return new SectionGraphVisualWidget(this, testUid, probeUid, channelId, sectionNumber, parent);
}
