#ifndef VISUALSFACTORY_H
#define VISUALSFACTORY_H

#include <QObject>

class VisualDescriptor;


/*!
 * \brief Класс фабрики классов визуализаторов The VisualsFactory class
 */
class VisualsFactory : public QObject
{
    Q_OBJECT
public:
    explicit VisualsFactory(QObject *parent = nullptr);

    void registerVisual(VisualDescriptor* visual);

    int testVisualsCount();
    VisualDescriptor* getTestVisual(const int idx);

    int probeVisualsCount();
    VisualDescriptor* getProbeVisual(const int idx);

    int channelVisualsCount();
    VisualDescriptor* getChannelVisual(const int idx);

private:
    QList<VisualDescriptor*> m_visTest;
    QList<VisualDescriptor*> m_visProbe;
    QList<VisualDescriptor*> m_visChannel;

};

#endif // VISUALSFACTORY_H
