#ifndef VISUALSFACTORY_H
#define VISUALSFACTORY_H

#include <QObject>

#include "basedefines.h"

class VisualDescriptor;


/*!
 * \brief Класс фабрики классов визуализаторов The VisualsFactory class
 */
class VisualsFactory : public QObject
{
    Q_OBJECT
public:
    explicit VisualsFactory(QObject *parent = nullptr);
    ~VisualsFactory() override;

    void registerVisual(VisualDescriptor* visual);

    int visualCount(const BaseDefines::TestLevel level);
    VisualDescriptor* getVisual(const BaseDefines::TestLevel level, const int idx);

private:
    QList<VisualDescriptor*> m_visTest;
    QList<VisualDescriptor*> m_visProbe;
    QList<VisualDescriptor*> m_visChannel;
    QList<VisualDescriptor*> m_visSections;

};

#endif // VISUALSFACTORY_H
