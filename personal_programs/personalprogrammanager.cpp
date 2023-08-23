#include "personalprogrammanager.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QUuid>
#include <QDebug>

#include "datadefines.h"
#include "personalprogramdefines.h"

PersonalProgramManager::PersonalProgramManager(QObject *parent)
    : QObject (parent)
{

}

void PersonalProgramManager::readDailyProgramList(QStandardItemModel &model, QStringList uids)
{
    model.clear();

    //! Создание папки
    QString dirName = createDir();

    //! Заполнение модели
    QFile fDP(dirName + "dp.json");
    if (fDP.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //! Чтение файла
        QByteArray ba = fDP.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(ba));
        auto objDPs = loadDoc.object();
        fDP.close();

        //! Функция создания итема
        auto addItem = [&](QJsonObject& objDP)
        {
            QString name = objDP["name"].toString();

            auto *item = new QStandardItem(name);
            item->setEditable(false);
            item->setData(objDP, PersonalProgramDefines::TableDPRoles::DPRole);
            model.appendRow(item);
        };

        //! Чтение массива
        auto arrDPs = objDPs["dp_list"].toArray();
        //! Если нужен весь файл
        if (uids == QStringList())
        {
            for (int i = 0; i < arrDPs.size(); ++i)
            {
                auto objDP = arrDPs.at(i).toObject();
                addItem(objDP);
            }
        }
        else
        //! Если нужны только указанные DP
        {
            foreach (auto uidDP, uids)
            {
                for (int i = 0; i < arrDPs.size(); ++i)
                {
                    auto objDP = arrDPs.at(i).toObject();
                    QString uid = objDP["uid"].toString();
                    if (uid == uidDP)
                    {
                        addItem(objDP);
                    }
                }
            }
        }
    }
}

QJsonArray PersonalProgramManager::readDailyProgramList(QStringList uids)
{
    QJsonArray retval;

    //! Создание папки
    QString dirName = createDir();

    //! Заполнение модели
    QFile fDP(dirName + "dp.json");
    if (fDP.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //! Чтение файла
        QByteArray ba = fDP.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(ba));
        auto objDPs = loadDoc.object();
        fDP.close();

//        //! Функция создания итема
//        auto addItem = [&](QJsonObject& objDP)
//        {
//            QString name = objDP["name"].toString();

//            auto *item = new QStandardItem(name);
//            item->setEditable(false);
//            item->setData(objDP, PersonalProgramDefines::TableDPRoles::DPRole);
//            model.appendRow(item);
//        };

        //! Чтение массива
        auto arrDPs = objDPs["dp_list"].toArray();
        //! Если нужен весь файл
        if (uids == QStringList())
        {
            for (int i = 0; i < arrDPs.size(); ++i)
            {
                auto objDP = arrDPs.at(i).toObject();
                retval << objDP;
//                addItem(objDP);
            }
        }
        else
        //! Если нужны только указанные DP
        {
            foreach (auto uidDP, uids)
            {
                for (int i = 0; i < arrDPs.size(); ++i)
                {
                    auto objDP = arrDPs.at(i).toObject();
                    QString uid = objDP["uid"].toString();
                    if (uid == uidDP)
                    {
                        retval << objDP;
//                        addItem(objDP);
                    }
                }
            }
        }
    }

    return retval;
}

void PersonalProgramManager::saveDailyProgramList(const QStandardItemModel &model)
{
    //! Создание json массива на основании модели
    QJsonObject objDPs;
    QJsonArray arrDPs;
    for (int i = 0; i < model.rowCount(); ++i)
    {
        auto dp = model.index(i, 0).data(PersonalProgramDefines::TableDPRoles::DPRole).toJsonObject();
        arrDPs << dp;
    }
    objDPs["dp_list"] = arrDPs;

    //! Создание папки
    QString dirName = createDir();

    //! Запись в файл
    QFile fDP(dirName + "dp.json");
    if (fDP.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QJsonDocument doc(objDPs);
        QByteArray ba = doc.toJson();
        fDP.write(ba);
    }
}

void PersonalProgramManager::readPersonalProgramList(QStandardItemModel &model)
{
    model.clear();

    //! Создание папки
    QString dirName = createDir();

    //! Заполнение модели
    QFile fPP(dirName + "pp.json");
    if (fPP.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //! Чтение файла
        QByteArray ba = fPP.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(ba));
        auto objPPs = loadDoc.object();
        fPP.close();

        //! Чтение массива
        auto arrPPs = objPPs["pp_list"].toArray();
        for (int i = 0; i < arrPPs.size(); ++i)
        {
            auto objPP = arrPPs.at(i).toObject();
            QString name = objPP["name"].toString();
            auto logoFileName = objPP["logo_file_name"].toString();

            auto *item = new QStandardItem(QIcon(logoFileName), name);
            item->setEditable(false);
            item->setData(objPP, PersonalProgramDefines::TablePPRoles::PPRole);
            model.appendRow(item);
        }
    }
}

void PersonalProgramManager::savePersonalProgramList(const QStandardItemModel &model)
{
    //! Создание json массива на базе модели
    QJsonObject objPPs;
    QJsonArray arrPPs;
    for (int i = 0; i < model.rowCount(); ++i)
    {
        auto pp = model.index(i, 0).data(PersonalProgramDefines::TablePPRoles::PPRole).toJsonObject();
        arrPPs << pp;
    }
    objPPs["pp_list"] = arrPPs;

    //! Создание папки
    QString dirName = createDir();

    //! Запись в файл
    QFile fDP(dirName + "pp.json");
    if (fDP.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QJsonDocument doc(objPPs);
        QByteArray ba = doc.toJson();
        fDP.write(ba);
    }
}

void PersonalProgramManager::assignDailyProgramsForPersonal(QString &uidPP, QStringList &uidDPs)
{
    //! Создание папки
    QString dirName = createDir();

    QJsonObject objPPDPs;

    //! Чтение файла
    QFile fPPDP(dirName + "ppdp.json");
    if (fPPDP.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray ba = fPPDP.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(ba));
        objPPDPs = loadDoc.object();
        fPPDP.close();
    }

    //! Создание json массива
    auto arrPPDP = objPPDPs["ppdp_list"].toArray();
    for (int i = 0; i < uidDPs.size(); ++i)
    {
        QJsonObject objPPDP;
        objPPDP["pp_uid"] = uidPP;
        objPPDP["dp_uid"] = uidDPs.at(i);
        objPPDP["number"] = i;

        arrPPDP << objPPDP;
    }
    objPPDPs["ppdp_list"] = arrPPDP;

    //! Запись в файл
    if (fPPDP.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QJsonDocument doc(objPPDPs);
        QByteArray ba = doc.toJson();
        fPPDP.write(ba);
    }
}

void PersonalProgramManager::editDailyProgramsForPersonal(QString &uidPP, QStringList &uidDPs)
{
    clearListDailyProgramsForPersonal(uidPP);
    assignDailyProgramsForPersonal(uidPP, uidDPs);
}

void PersonalProgramManager::clearListDailyProgramsForPersonal(QString &uidPP)
{
    //! Создание папки
    QString dirName = createDir();

    QJsonObject objPPDPs;

    //! Чтение файла
    QFile fPPDP(dirName + "ppdp.json");
    if (fPPDP.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray ba = fPPDP.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(ba));
        objPPDPs = loadDoc.object();
        fPPDP.close();
    }

    //! Создание json массива
    auto arrPPDP = objPPDPs["ppdp_list"].toArray();
    QJsonArray arrPPDPNew;
    for (int i = 0; i < arrPPDP.size(); ++i)
    {
        auto objPPDP = arrPPDP.at(i).toObject();
        if (objPPDP["pp_uid"] != uidPP)
            arrPPDPNew << objPPDP;
    }
    objPPDPs["ppdp_list"] = arrPPDPNew;

    //! Запись в файл
    if (fPPDP.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QJsonDocument doc(objPPDPs);
        QByteArray ba = doc.toJson();
        fPPDP.write(ba);
    }
}

QStringList PersonalProgramManager::getListDailyProgramsForPersonal(QString &uidPP)
{
    //! Создание папки
    QString dirName = createDir();

    //! Объект, в который помещены данные файла в виде json
    QJsonObject objPPDPs;

    //! Прочитаем файл ppdp.json
    QFile fPPDP(dirName + "ppdp.json");
    if (fPPDP.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray ba = fPPDP.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(ba));
        objPPDPs = loadDoc.object();
        fPPDP.close();
    }

    //! Структура для сортировки данных
    struct PPDPVal
    {
        QString uid;
        int number;
    };
    QList<PPDPVal> values;

    //! Проходим по всем данным, выбирая, относящиеся к нужной PP, используя uidPP
    auto arrPPDP = objPPDPs["ppdp_list"].toArray();
    for (int i = 0; i < arrPPDP.size(); ++i)
    {
        auto objPPDP = arrPPDP.at(i).toObject();
        if (objPPDP["pp_uid"].toString() == uidPP)
        {
            PPDPVal val;
            val.uid = objPPDP["dp_uid"].toString();
            val.number = objPPDP["number"].toInt();
            values << val;
        }
    }
    //! Сортируем
    std::sort(values.begin(), values.end(), [](PPDPVal& a, PPDPVal&b) {return a.number < b.number;});

    //! Переливаем в массив для возврата
    QStringList retval;
    for (int i = 0; i < values.size(); ++i)
        retval << values.at(i).uid;
    return retval;
}

QJsonObject PersonalProgramManager::assignPersonalProgramForPatient(const QString &patientUid,
                                                                    const QString &ppUid,
                                                                    QString& ppUidAssigned)
{
    //! uid назначенной PP
    ppUidAssigned = QUuid::createUuid().toString();

    //! Сборка PP в один json
    QJsonObject retval;
    retval["pp_uid"] = ppUid;                //! uid исходной PP
    retval["patient_uid"] = patientUid;      //! uid пациента
    retval["assigned_uid"] = ppUidAssigned;  //! uid назначенной PP
    retval["active"] = true;
    retval["date_begin"] = QDate::currentDate().toString("dd.MM.yyyy");

    //! Создание папки
    QString dirName = createDir();

    //! Чтение индивидуальной программы
    QFile fPP(dirName + "pp.json");
    if (fPP.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //! Чтение файла
        QByteArray ba = fPP.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(ba));
        auto objPPs = loadDoc.object();
        fPP.close();

        //! Чтение массива
        auto arrPPs = objPPs["pp_list"].toArray();
        for (int i = 0; i < arrPPs.size(); ++i)
        {
            auto objPP = arrPPs.at(i).toObject();
            QString uid = objPP["uid"].toString();

            //! Выделяем PP с заданным uid (ppUid)
            if (uid == ppUid)
            {
                //! Получим список дневных программ для индивидуальной
                auto dpList = getListDailyProgramsForPersonal(uid);
                //! Создадим список в виде массива реальных дневных программ
                auto arrDP = readDailyProgramList(dpList);
                //! Соберем все в выходной объект
                objPP["dp_list"] = arrDP;
                retval["pp"] = objPP;
            }
        }
    }

    return retval;
}

QString PersonalProgramManager::createDir()
{
    QString dirName = DataDefines::appCopyPath() + "personal_programs/";
    QDir dir(dirName);
    if (!dir.exists())
        dir.mkpath(dirName);
    return dirName;
}
