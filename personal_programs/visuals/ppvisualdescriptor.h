#ifndef PPVISUALDESCRIPTOR_H
#define PPVISUALDESCRIPTOR_H

#include <QObject>
#include <QJsonObject>

class PPVisual;

/*!
 * \brief Класс описателя визуализаторов индивидуальных программ The PPVisualDescriptor class
 */
class PPVisualDescriptor
{
public:
    PPVisualDescriptor();
    virtual ~PPVisualDescriptor();

    virtual QString uid() = 0;
    virtual QString name() = 0;

    virtual PPVisual* getVisualWidget(QJsonObject &objPP, QWidget *parent = nullptr) = 0;
};

#endif // PPVISUALDESCRIPTOR_H
