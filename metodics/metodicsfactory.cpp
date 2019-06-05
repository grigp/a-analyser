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

MetodicDefines::MetodicInfo MetodicsFactory::metodic(const QString &metUid) const
{
    auto mi = getMetodicIndexByUid(metUid);
    return metodic(mi);
}

bool MetodicsFactory::editMetodicParams(QWidget *parent, const QString &metUid)
{
    auto *mt = getMetodicTemplate(metUid);
    if (mt)
    {
        auto mi = getMetodicIndexByUid(metUid);
        if (mi > -1)
        {
            auto met = m_metodics.at(mi);
            bool retval = mt->editParams(parent, met.params);
            if (retval)
            {
                m_metodics.replace(mi, met);
                saveMetodics();
            }
            return retval;
        }
    }
    return false;
}

void MetodicsFactory::execute(QWidget *parent, const QString &metUid) const
{
    auto *mt = getMetodicTemplate(metUid);
    if (mt)
    {
        auto mi = getMetodicIndexByUid(metUid);
        if (mi > -1)
        {
            mt->execute(parent, m_metodics.at(mi).params);
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
    fMet.setPermissions((((fMet.permissions() |= QFile::WriteOwner) |= QFile::WriteUser) |= QFile::WriteGroup) |= QFile::WriteOther);
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
            mi.uid = met["uid"].toString(); //QUuid::createUuid().toString();
            mi.name = met["name"].toString();
            mi.templateId = met["template"].toString();
            mi.params = met["params"].toObject();

            m_metodics << mi;
        }
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
            obj["uid"] = met.uid;
            obj["name"] = met.name;
            obj["template"] = met.templateId;
            obj["params"] = met.params;
            tests.append(obj);
        }
        root["tests"] = tests;

        QJsonDocument doc(root);
        QByteArray ba = doc.toJson();
        fMet.write(ba);
        }
}

int MetodicsFactory::getMetodicIndexByUid(const QString &uid) const
{
    for (int i = 0; i < m_metodics.size(); ++i)
        if (m_metodics.at(i).uid == uid)
            return i;
    return -1;
}

MetodicTemplate *MetodicsFactory::getMetodicTemplate(const QString &metUid) const
{
    int metIdx = getMetodicIndexByUid(metUid);
    if (metIdx > -1)
    {
        foreach (auto templ, m_templates)
            if (templ->uid() == m_metodics.at(metIdx).templateId)
                return templ;
    }
    return nullptr;
}

