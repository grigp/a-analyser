#include "korrelationritmogramvisual.h"

#include "korrelationritmogramvisualwidget.h"

KorrelationRitmogramVisual::KorrelationRitmogramVisual(BaseDefines::TestLevel level)
    : VisualDescriptor (level)
{

}

QString KorrelationRitmogramVisual::uid()
{
    return VisualNames::UID_KRG;
}

QString KorrelationRitmogramVisual::name()
{
    return QCoreApplication::tr("Корреляционная ритмограмма"); //VisualNames::Name_KRG;
}

Visual *KorrelationRitmogramVisual::getVisualWidget(QWidget *parent,
                                                    const QString &testUid,
                                                    const QString &probeUid,
                                                    const QString &channelId,
                                                    const QString &sectionNumber)
{
    Q_UNUSED(sectionNumber);
    return new KorrelationRitmogramVisualWidget(this, testUid, probeUid, channelId, parent);
}
