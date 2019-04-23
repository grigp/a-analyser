#include "metodicsfactory.h"

#include "metodictemplate.h"
#include "stabtesttemplate.h"
#include "jumptesttemplate.h"
#include "datadefines.h"

#include <QFile>
#include <QDir>
#include <QUuid>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>

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

bool MetodicsFactory::editMetodicParams(QWidget *parent, const QString &metUid)
{
    int metIdx = getMetodicIndexByUid(metUid);
    if (metIdx > -1)
    {
        MetodicTemplate* mt = nullptr;
        foreach (auto templ, m_templates)
            if (templ->uid() == m_metodics.at(metIdx).templateId)
            {
                mt = templ;
                break;
            }

        if (mt)
        {
            auto met = m_metodics.at(metIdx);
            bool retval = mt->editParams(parent, met.params);
            if (retval)
            {
                m_metodics.replace(metIdx, met);
                saveMetodics();
            }
            return retval;
        }
    }
}

void MetodicsFactory::assignTemplates()
{

    m_templates << new StabTestTemplate(this)
                << new JumpTestTemplate(this);
}

void MetodicsFactory::assignMetodics()
{
    //! Файл со списком и параметрами методик берем из ресурса, создаем его на диске,
    //! после чего читаем и редактируем уже с диска.
    //! Это позволяет не инсталить ни методики, ни базу данных, а создавать все в
    //! в процессе работы
    if (!QFile::exists(DataDefines::appDataPath() + "metodics.json"))
        QFile::copy(":/metodics.json",
                    DataDefines::appDataPath() + "metodics.json");

    QFile fMet(DataDefines::appDataPath() + "metodics.json");
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
            mi.uid = QUuid::createUuid().toString();
            mi.name = met["name"].toString();
            mi.templateId = met["template"].toString();
            mi.params = met["params"].toObject();

            m_metodics << mi;
        }

        fMet.close();
    }
}

void MetodicsFactory::saveMetodics()
{
    QDir dir = DataDefines::appDataPath();
    QFile fMet(dir.absoluteFilePath("metodics.json"));
    if (fMet.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QJsonObject root;
        QJsonArray tests;
        foreach (auto met, m_metodics)
        {
            QJsonObject obj;
            obj["name"] = met.name;
            obj["template"] = met.templateId;
            obj["params"] = met.params;
            tests.append(obj);
        }
        root["tests"] = tests;

        QJsonDocument doc(root);
        QByteArray ba = doc.toJson();
        fMet.write(ba);
        fMet.close();
        }
}

int MetodicsFactory::getMetodicIndexByUid(const QString &uid) const
{
    for (int i = 0; i < m_metodics.size(); ++i)
        if (m_metodics.at(i).uid == uid)
            return i;
    return -1;
}

