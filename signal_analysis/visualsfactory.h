#ifndef VISUALSFACTORY_H
#define VISUALSFACTORY_H

#include <QObject>

class BaseVisual;
class TestVisual;
class ProbeVisual;
class ChannelVisual;

/*!
 * \brief Класс фабрики классов визуализаторов The VisualsFactory class
 */
class VisualsFactory : public QObject
{
    Q_OBJECT
public:
    explicit VisualsFactory(QObject *parent = nullptr);

    void registerVisual(BaseVisual* visual);

    int testVisualsCount();
    TestVisual* getTestVisual(const int idx);

    int probeVisualsCount();
    TestVisual* getProbeVisual(const int idx);

    int channelVisualsCount();
    TestVisual* getChannelVisual(const int idx);

private:
    QList<TestVisual*> m_visTest;
    QList<ProbeVisual*> m_visProbe;
    QList<ChannelVisual*> m_visChannel;

};

#endif // VISUALSFACTORY_H
