#include "signaltransformfactory.h"

#include <QTimer>

#include "signaltransformer.h"
#include "aanalyserbuild.h"

SignalTransformFactory::SignalTransformFactory(QObject *parent) : QObject(parent)
{
    m_transformers.clear();

    QTimer::singleShot(0, [=]()
    {
        AAnalyserBuild::registerSignalTransformers();
    });
}

void SignalTransformFactory::registerTransformer(SignalTransformer *st)
{
    m_transformers << st;
}

int SignalTransformFactory::count()
{
    return m_transformers.size();
}

QString SignalTransformFactory::uid(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < m_transformers.size());
    return m_transformers.at(idx)->uid();
}

QString SignalTransformFactory::name(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < m_transformers.size());
    return m_transformers.at(idx)->name();
}

void SignalTransformFactory::transform(const int idx, QVector<double> &buffer) const
{
    Q_ASSERT(idx >= 0 && idx < m_transformers.size());
    m_transformers.at(idx)->transform(buffer);
}

void SignalTransformFactory::transform(const QString uid, QVector<double> &buffer) const
{
    foreach (auto transformer, m_transformers)
    {
        if (transformer->uid() == uid)
        {
            transformer->transform(buffer);
            break;
        }
    }
}
