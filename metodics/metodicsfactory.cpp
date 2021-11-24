#include "metodicsfactory.h"

#include "aanalyserapplication.h"
#include "aanalyserdefines.h"
#include "metodictemplate.h"
#include "stabtesttemplate.h"
#include "extend3dgamestemplate.h"
#include "jumptesttemplate.h"
#include "jumpheighttesttemplate.h"
#include "teppingtesttemplate.h"
#include "trentakeputtemplate.h"
#include "trentetristemplate.h"
#include "droptesttemplate.h"
#include "crosstemplate.h"
#include "stepoffsettemplate.h"
#include "stepdeviationtemplate.h"
#include "evolventatemplate.h"
#include "octaedrontemplate.h"
#include "boxerdodgingtemplate.h"
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

int MetodicsFactory::metodicKindsCount() const
{
    return m_metodicKinds.size();
}

MetodicDefines::MetodicKindInfo MetodicsFactory::metodicKind(const int i) const
{
    Q_ASSERT(i >= 0 && i < m_metodicKinds.size());
    MetodicDefines::MetodicKindInfo retval = m_metodicKinds.at(i);
    return retval;
}

MetodicDefines::MetodicKindInfo MetodicsFactory::metodicKind(const QString &kindUid) const
{
    auto mki = getMetodicKindIndexByUid(kindUid);
    return metodicKind(mki);
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

void MetodicsFactory::print(QPrinter *printer, const QString &testUid) const
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
                return mt->print(printer, testUid);
            }
        }
    }
}

void MetodicsFactory::assignTemplates()
{

    m_templates << new StabTestTemplate(this)
                << new Extend3DGamesTemplate(this)
                << new JumpTestTemplate(this)
                << new JumpHeightTestTemplate(this)
                << new TeppingTestTemplate(this)
                << new DropTestTemplate(this)
                << new TrenTetrisTemplate(this)
                << new TrenTakePutTemplate(this)
                << new CrossTemplate(this)
                << new StepOffsetTemplate(this)
                << new StepDeviationTemplate(this)
                << new EvolventaTemplate(this)
                << new OctaedronTemplate(this)
                << new BoxerDodgingTemplate(this);

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

    if (AAnalyserDefines::isAutoImportDatabases)
        assignMetodicsFromAutoImport();

    QFile fMet(DataDefines::appCopyPath() + "metodics.json");
    fMet.setPermissions((((fMet.permissions() |= QFile::WriteOwner) |= QFile::WriteUser) |= QFile::WriteGroup) |= QFile::WriteOther);
    if (fMet.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray ba = fMet.readAll();
        QJsonDocument doc(QJsonDocument::fromJson(ba));
        QJsonObject root = doc.object();

        //! Методики
        QJsonArray mets = root["tests"].toArray();
        for (int i = 0; i < mets.size(); ++i)
        {
            auto met = mets[i].toObject();
            MetodicDefines::MetodicInfo mi;
            mi.uid = met["uid"].toString();
            mi.name = met["name"].toString();
            mi.imageName = met["image"].toString();
            mi.templateId = met["template"].toString();
            mi.kindUid = met["kind"].toString();
            mi.buildNorms = met["build_norms"].toBool();
            mi.params = met["params"].toObject();

            m_metodics << mi;
        }

        //! Типы методик
        QJsonArray kinds = root["tests_kinds"].toArray();
        for (int i = 0; i < kinds.size(); ++i)
        {
            auto kind = kinds[i].toObject();
            MetodicDefines::MetodicKindInfo mki;
            mki.uid = kind["uid"].toString();
            mki.name = kind["name"].toString();
            mki.imageName = kind["image"].toString();

            m_metodicKinds << mki;
        }
    }
}

void MetodicsFactory::assignMetodicsFromAutoImport()
{
    QString path = DataDefines::amedDataPath();
    //Цикл по пути path + "Shared\AutoImport\" с поиском файлов *.json и импорт из них методик
    QDir dir(path + "Shared/AutoImport/");
    QFileInfoList list = dir.entryInfoList(); //QDir::NoDotAndDotDot);
    foreach (auto fileInfo, list)
        if (fileInfo.fileName() != "." && fileInfo.fileName() != "..")
        {
            if (fileInfo.completeSuffix() == "json")
                appendNewMetodic(path + "Shared/AutoImport/" + fileInfo.fileName(),
                                 DataDefines::appCopyPath() + "metodics.json");
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
            obj["image"] = met.imageName;
            obj["template"] = met.templateId;
            obj["kind"] = met.kindUid;
            obj["build_norms"] = met.buildNorms;
            obj["params"] = met.params;
            tests.append(obj);
        }
        root["tests"] = tests;

        QJsonArray tests_kinds;
        foreach (auto kind, m_metodicKinds)
        {
            QJsonObject obj;
            obj["uid"] = kind.uid;
            obj["name"] = kind.name;
            obj["image"] = kind.imageName;
            tests_kinds.append(obj);
        }
        root["tests_kinds"] = tests_kinds;

        QJsonDocument doc(root);
        QByteArray ba = doc.toJson();
        fMet.write(ba);
    }
}

void MetodicsFactory::appendNewMetodic(const QString &fnPreDefMetodics, const QString &fnMetodics)
{
    auto metPD = readMetodicsFile(fnPreDefMetodics, "tests");
    auto met = readMetodicsFile(fnMetodics, "tests");
    auto kindPD = readMetodicsFile(fnPreDefMetodics, "tests_kinds");
    auto kind = readMetodicsFile(fnMetodics, "tests_kinds");

    bool chngMet = appendInArray(met, metPD, msMethodic);
    bool chngKind = appendInArray(kind, kindPD, msMethodicKind);
    if (chngMet || chngKind)
        writeMetodicsFile(met, kind, fnMetodics);
}

bool MetodicsFactory::appendInArray(QJsonArray &arr, QJsonArray &arrPD, const MethodicSubject ms)
{
    QList<int> newIdxList;
    bool retval = false;

    for (int i = 0; i < arrPD.size(); ++i)
    {
        auto objMetPD = arrPD.at(i).toObject();

        //! Поиск методики в перечне шаблонов
        bool fnd = true;
        if (ms == msMethodic)
            fnd = isTemplateExists(objMetPD["template"].toString());

        //! Нашли - можно искать в уже подключенных
        if (fnd)
        {
            //! Поиск методики в перечне подключенных
            fnd = false;
            for (int j = 0; j < arr.size(); ++j)
            {
                auto objMet = arr.at(j).toObject();
                if (objMet["uid"].toString() == objMetPD["uid"].toString())
                {
                    fnd = true;
                    break;
                }
            }
            if (!fnd)
                newIdxList << i;
        }
    }

    if (newIdxList.size() > 0)
    {
        foreach (auto nmIdx, newIdxList)
        {
            auto objMetPD = arrPD.at(nmIdx).toObject();
            arr.append(objMetPD);
            retval = true;
        }
    }

    return retval;
}

int MetodicsFactory::getMetodicIndexByUid(const QString &uid) const
{
    for (int i = 0; i < m_metodics.size(); ++i)
        if (m_metodics.at(i).uid == uid)
            return i;
    return -1;
}

int MetodicsFactory::getMetodicKindIndexByUid(const QString &uid) const
{
    for (int i = 0; i < m_metodicKinds.size(); ++i)
        if (m_metodicKinds.at(i).uid == uid)
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

void MetodicsFactory::writeMetodicsFile(const QJsonArray &arrMethods, const QJsonArray &arrKinds, const QString &fn)
{
    QFile fMetodics(fn);
    fMetodics.setPermissions((((fMetodics.permissions() |= QFile::WriteOwner) |= QFile::WriteUser) |= QFile::WriteGroup) |= QFile::WriteOther);
    if (fMetodics.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QJsonObject obj;
        obj["tests"] = arrMethods;
        obj["tests_kinds"] = arrKinds;

        QJsonDocument doc(obj);
        QByteArray ba = doc.toJson();
        fMetodics.write(ba);
        fMetodics.close();
    }
}

QJsonArray MetodicsFactory::readMetodicsFile(const QString &fn, const QString &secName)
{
    QFile fMetodics(fn);
    fMetodics.setPermissions((((fMetodics.permissions() |= QFile::WriteOwner) |= QFile::WriteUser) |= QFile::WriteGroup) |= QFile::WriteOther);
    if (fMetodics.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray ba = fMetodics.readAll();
        QJsonDocument doc(QJsonDocument::fromJson(ba));
        QJsonObject obj = doc.object();
        fMetodics.close();

        return obj[secName].toArray();
    }
    return QJsonArray();
}

bool MetodicsFactory::isTemplateExists(const QString templateUID) const
{
    foreach (auto tmplt, m_templates)
        if (tmplt->uid() == templateUID)
            return true;
    return  false;
}


