#include "metodicsfactory.h"

#include "aanalyserapplication.h"
#include "aanalyserdefines.h"
#include "metodictemplate.h"
#include "stabtesttemplate.h"
#include "jumptesttemplate.h"
#include "datadefines.h"
#include "dataprovider.h"

#include <QFile>
#include <QDir>
#include <QUuid>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTimer>
#include <QDebug>

MetodicsFactory::MetodicsFactory(QObject *parent) : QObject(parent)
{
    QTimer::singleShot(0, [=]()
    {
        assignTemplates();
        assignMetodics();
    });
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

QWidget *MetodicsFactory::visualize(QWidget *parent, const QString &testUid) const
{
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
    {
        auto *mt = getMetodicTemplate(ti.metodUid);
        if (mt)
        {
            auto mi = getMetodicIndexByUid(ti.metodUid);
            if (mi > -1)
            {
                return mt->visualize(parent, testUid);
            }
        }
    }
    return nullptr;
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
    QDir dir(DataDefines::appCopyPath());
    if (!dir.exists())
        dir.mkpath(DataDefines::appCopyPath());
    QString resName = ":/pre_settings/" + AAnalyserDefines::PresetsMetodicsFileName + ".json";
    if (static_cast<AAnalyserApplication*>(QApplication::instance())->languargeCode() == DataDefines::LANG_CODE_ENGUSA)
        resName = ":/pre_settings/" + AAnalyserDefines::PresetsMetodicsFileName + "_en_US.json";

    if (!QFile::exists(DataDefines::appCopyPath() + "metodics.json"))
        QFile::copy(resName, DataDefines::appCopyPath() + "metodics.json");
    else
        appendNewMetodic(resName, DataDefines::appCopyPath() + "metodics.json");

    QFile fMet(DataDefines::appCopyPath() + "metodics.json");
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
            mi.uid = met["uid"].toString();
            mi.name = met["name"].toString();
            mi.templateId = met["template"].toString();
            mi.buildNorms = met["build_norms"].toBool();
            mi.params = met["params"].toObject();

            m_metodics << mi;
        }
    }
}

void MetodicsFactory::saveMetodics()
{
    QDir dir = DataDefines::appCopyPath();
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

void MetodicsFactory::appendNewMetodic(const QString &fnPreDefMetodics, const QString &fnMetodics)
{
    auto metPD = readMetodicsFile(fnPreDefMetodics);
    auto met = readMetodicsFile(fnMetodics);

    //! Если кол-во элементов в массивах равно, то и добавлять ничего не надо
    //! Методики будут только добавляться
    if (met.size() == metPD.size())
        return;

    QList<int> newMetIdx;

    for (int i = 0; i < metPD.size(); ++i)
    {
        auto objMetPD = metPD.at(i).toObject();
        bool fnd = false;
        for (int j = 0; j < met.size(); ++j)
        {
            auto objMet = met.at(j).toObject();
            if (objMet["uid"].toString() == objMetPD["uid"].toString())
            {
                fnd = true;
                break;
            }
        }
        if (!fnd)
            newMetIdx << i;
    }

    if (newMetIdx.size() > 0)
    {
        foreach (auto nmIdx, newMetIdx)
        {
            auto objMetPD = metPD.at(nmIdx).toObject();
            met.append(objMetPD);
        }
        writeMetodicsFile(met, fnMetodics);
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

void MetodicsFactory::writeMetodicsFile(const QJsonArray &arr, const QString &fn)
{
    QFile fMetodics(fn);
    if (fMetodics.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QJsonObject obj;
        obj["tests"] = arr;

        QJsonDocument doc(obj);
        QByteArray ba = doc.toJson();
        fMetodics.write(ba);
        fMetodics.close();
    }
}

QJsonArray MetodicsFactory::readMetodicsFile(const QString &fn)
{
    QFile fMetodics(fn);
    if (fMetodics.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray ba = fMetodics.readAll();
        QJsonDocument doc(QJsonDocument::fromJson(ba));
        QJsonObject obj = doc.object();
        fMetodics.close();

        return obj["tests"].toArray();
    }
    return QJsonArray();
}

