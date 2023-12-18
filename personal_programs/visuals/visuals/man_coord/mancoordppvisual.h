#ifndef MANCOORDPPVISUAL_H
#define MANCOORDPPVISUAL_H

#include <QObject>

#include "ppvisualdescriptor.h"

namespace PPVisualNames
{
static const QString UID_ManCoord = "ppvManCoord";
static const QString Name_ManCoord = "Оценка координационных способностей";
}


/*!
 * \brief Класс описателя визуализатора ИП - оценка координационных способностей человека The ManCoordPPVisual class
 */
class ManCoordPPVisual : public PPVisualDescriptor
{
public:
    ManCoordPPVisual();
    ~ManCoordPPVisual() override;

    QString uid() override;
    QString name() override;

    PPVisual* getVisualWidget(QJsonObject &objPP, QWidget *parent = nullptr) override;
};

#endif // MANCOORDPPVISUAL_H
