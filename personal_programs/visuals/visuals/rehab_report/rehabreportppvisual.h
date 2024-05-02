#ifndef REHABREPORTPPVISUAL_H
#define REHABREPORTPPVISUAL_H

#include <QObject>

#include "ppvisualdescriptor.h"

namespace PPVisualNames
{
static const QString UID_RehabReport = "ppvRehabReport";
static const QString Name_RehabReport = "Отчет";
}


/*!
 * \brief Класс описателя визуализатора ИП - отчет по курсу реабилитации The RehabReportPPVisual class
 */
class RehabReportPPVisual : public PPVisualDescriptor
{
public:
    RehabReportPPVisual();
    ~RehabReportPPVisual() override;

    QString uid() override;
    QString name() override;

    PPVisual* getVisualWidget(QJsonObject &objPP, QWidget *parent = nullptr) override;
};

#endif // REHABREPORTPPVISUAL_H
