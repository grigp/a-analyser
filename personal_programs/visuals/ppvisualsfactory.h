#ifndef PPVISUALSFACTORY_H
#define PPVISUALSFACTORY_H

#include <QObject>

class PPVisualDescriptor;

/*!
 * \brief Класс фабрики классов визуализаторов индивидуальных программ The PPVisualsFactory class
 */
class PPVisualsFactory : public QObject
{
    Q_OBJECT
public:
    explicit PPVisualsFactory(QObject *parent = nullptr);
    ~PPVisualsFactory() override;

    void registerVisual(PPVisualDescriptor* visual);

    int visualCount();
    PPVisualDescriptor* getVisual(const int idx);

private:
    QList<PPVisualDescriptor*> m_visuals;
};

#endif // PPVISUALSFACTORY_H
