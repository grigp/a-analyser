#include "metodicsfactory.h"

#include "metodictemplate.h"
#include "stabtesttemplate.h"
#include "jumptesttemplate.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>

MetodicsFactory::MetodicsFactory(QObject *parent) : QObject(parent)
{
    assignTemplates();
    assignMetodics();
}

MetodicsFactory::~MetodicsFactory()
{
    m_templates.clear();
}

int MetodicsFactory::metodicsCount() const
{
    return m_metodics.size();
}

MetodicDefines::MetodicInfo MetodicsFactory::metodic(const int i) const
{
    Q_ASSERT(i >= 0 && i < m_metodics.size());
    MetodicDefines::MetodicInfo retval = m_metodics.at(i);
    return retval;
}

void MetodicsFactory::assignTemplates()
{
    m_templates << new StabTestTemplate(this)
                << new JumpTestTemplate(this);
}

void MetodicsFactory::assignMetodics()
{
    QFile fMet(":/metodics.json");
    if (fMet.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray ba = fMet.readAll();
        QJsonDocument doc(QJsonDocument::fromJson(ba));
        QJsonObject root = doc.object();
        QJsonArray mets = root["tests"].toArray();

        for (int i = 0; i < mets.size(); ++i)
        {
            auto met = mets[i].toObject();
            MetodicDefines::MetodicInfo mi;
            mi.name = met["name"].toString();
            mi.templateId = met["name"].toString();
            mi.params = met["params"].toObject();

            m_metodics << mi;
        }


        fMet.close();
    }
}
