#ifndef COMMONPPVISUAL_H
#define COMMONPPVISUAL_H

#include <QObject>

#include "ppvisualdescriptor.h"

namespace PPVisualNames
{
static const QString UID_Common = "ppvCommon";
static const QString Name_Common = "Анализ индивидуальной программы";
}


/*!
 * \brief Класс описателя визуализатора ИП - анализ результатов ИП The CommonPPVisual class
 */
class CommonPPVisual : public PPVisualDescriptor
{
public:
    CommonPPVisual();
    ~CommonPPVisual() override;

    QString uid() override;
    QString name() override;

    PPVisual* getVisualWidget(QJsonObject &objPP, QWidget *parent = nullptr) override;
};

#endif // COMMONPPVISUAL_H
