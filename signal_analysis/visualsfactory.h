#ifndef VISUALSFACTORY_H
#define VISUALSFACTORY_H

#include <QObject>

#include "visualdefines.h"

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

    int visualCount(const VisualDefines::Level level);
    VisualDescriptor* getVisual(const VisualDefines::Level level, const int idx);

private:
    QList<VisualDescriptor*> m_visTest;
    QList<VisualDescriptor*> m_visProbe;
    QList<VisualDescriptor*> m_visChannel;

};

#endif // VISUALSFACTORY_H
