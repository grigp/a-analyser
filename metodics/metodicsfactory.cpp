#include "metodicsfactory.h"

#include "aanalyserapplication.h"
#include "aanalyserdefines.h"
#include "metodictemplate.h"
#include "aanalyserbuild.h"
#include "datadefines.h"
#include "dataprovider.h"
#include "testcalculator.h"

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
    m_calculators.clear();
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
    if (mi >= 0)
        return metodic(mi);
    return MetodicDefines::MetodicInfo();
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
            bool retval = mt->editParams(parent, met.params, met.name);
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

bool MetodicsFactory::editMetodicParams(QWidget *parent, const QString &metUid, QJsonObject &params)
{
    auto *mt = getMetodicTemplate(metUid);
    if (mt)
    {
        auto mi = getMetodicIndexByUid(metUid);
        if (mi > -1)
        {
            bool retval = mt->editParams(parent, params);
            return retval;
        }
    }
    return false;
}

QJsonObject MetodicsFactory::setMetodicParamsDefault(const QString &uidMet)
{
    //! Имя файла дефолтных методик
    QString resName = ":/pre_settings/" + AAnalyserDefines::PresetsMetodicsFileName + ".json";
    if (static_cast<AAnalyserApplication*>(QApplication::instance())->languargeCode() == DataDefines::LANG_CODE_ENGUSA)
        resName = ":/pre_settings/" + AAnalyserDefines::PresetsMetodicsFileName + "_en_US.json";

    //! Чтение тестов из файла
    auto metPD = readMetodicsFile(resName, "tests");
    for (int i = 0; i < metPD.size(); ++i)
    {
        auto objTest = metPD.at(i).toObject();
        //! Нужная методика
        if (uidMet == objTest["uid"].toString())
        {
            auto defParams = objTest["params"].toObject();

//            //! Методика во внутреннем списке
//            auto mi = getMetodicIndexByUid(uidMet);
//            if (mi > -1)
//            {
//                //! Заменить параметры
//                auto met = m_metodics.at(mi);
//                met.params = defParams;
//                m_metodics.replace(mi, met);
//                //! Сохранить на диске
//                saveMetodics();
//            }

            //! Вернуть параметры
            return defParams;
        }
    }
    return QJsonObject();
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

void MetodicsFactory::execute(QWidget *parent, const QString &metUid, const QJsonObject &params) const
{
    auto *mt = getMetodicTemplate(metUid);
    if (mt)
    {
        auto mi = getMetodicIndexByUid(metUid);
        if (mi > -1)
        {
            mt->execute(parent, params);
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

void MetodicsFactory::paintPreview(QPainter *painter, QRect &rect, const QString &testUid) const
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
                mt->paintPreview(painter, rect, testUid);
            }
        }
    }
}

void MetodicsFactory::storeCalculator(const QString &uidTest, TestCalculator *calculator)
{
    m_calculators.insert(uidTest, calculator);
}

TestCalculator *MetodicsFactory::getCalculator(const QString &uidTest)
{
    if (m_calculators.contains(uidTest))
        return m_calculators.value(uidTest);
    return nullptr;
}

QString MetodicsFactory::autoModeStageTitle(MetodicDefines::AutoModeStaticStages stage)
{
    /*!
     * \brief Заголовки этапов статической пробы для автоматического режима The AutoModeStageTitle
     */
    static QMap<MetodicDefines::AutoModeStaticStages, QString> AutoModeStageTitle {
          std::pair<MetodicDefines::AutoModeStaticStages, QString> (MetodicDefines::amssLatent0, QCoreApplication::tr(""))
        , std::pair<MetodicDefines::AutoModeStaticStages, QString> (MetodicDefines::amssZeroingWait, QCoreApplication::tr("Центровка через"))
        , std::pair<MetodicDefines::AutoModeStaticStages, QString> (MetodicDefines::amssLatent1, QCoreApplication::tr(""))
        , std::pair<MetodicDefines::AutoModeStaticStages, QString> (MetodicDefines::amssRecordingWait, QCoreApplication::tr("Запись через"))
        , std::pair<MetodicDefines::AutoModeStaticStages, QString> (MetodicDefines::amssRecording, QCoreApplication::tr(""))
    };
    return AutoModeStageTitle.value(stage);
}

void MetodicsFactory::assignTemplates()
{
    m_templates << AAnalyserBuild::getBuildTemplates(this);
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
            mi.shortName = met["short_name"].toString();
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
            ///TODO: Сделать прием и разбор в зависимости от локали
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
            obj["short_name"] = met.shortName;
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
    bool retval = true; //false; Раньше перезаписывали только для новой методики, а теперь для всех с учетом того, что и другие параметры могут поменяться

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
                    //! Для совпадающих методик обновим все поля, кроме параметров, ибо,
                    //! если внесли изменения или добавилось что-то, оно должно присутствовать
                    if (msMethodic == ms)  //! Методики
                    {
                        objMet["build_norms"] = objMetPD["build_norms"].toBool();
                        objMet["image"] = objMetPD["image"].toString();
                        objMet["kind"] = objMetPD["kind"].toString();
                        objMet["name"] = objMetPD["name"].toString();
                        objMet["short_name"] = objMetPD["short_name"].toString();
                        objMet["template"] = objMetPD["template"].toString();
                    }
                    else
                    if (msMethodicKind == ms)  //! Типы методик
                    {
                        objMet["image"] = objMetPD["image"].toString();
                        objMet["name"] = objMetPD["name"].toString();
                    }
                    arr.replace(j, objMet);
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


