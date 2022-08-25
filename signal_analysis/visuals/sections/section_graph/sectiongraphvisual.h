#ifndef SECTIONGRAPHVISUAL_H
#define SECTIONGRAPHVISUAL_H

#include <QObject>

#include "basedefines.h"
#include "visualdescriptor.h"

namespace VisualNames
{
static const QString UID_SectionGraph = "vSectionGraph";
static const QString Name_SectionGraph = "Анализ сигналов";
}

/*!
 * \brief Класс визуализатора графика секции The SectionGraphVisual class
 */
class SectionGraphVisual : public VisualDescriptor
{
public:
    SectionGraphVisual(BaseDefines::TestLevel level);
    ~SectionGraphVisual() override;

    QString uid() override;
    QString name() override;

    Visual* getVisualWidget(QWidget *parent = nullptr,
                            const QString& testUid = "",
                            const QString& probeUid = "",
                            const QString& channelId = "",
                            const QString& sectionNumber = "") override;
};

#endif // SECTIONGRAPHVISUAL_H
