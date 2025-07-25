#include "personalprogrammanager.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QUuid>
#include <QVariant>
#include <QDebug>

#include "aanalyserdefines.h"
#include "aanalyserapplication.h"
#include "datadefines.h"
#include "personalprogramdefines.h"
#include "aanalyserbuild.h"

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

void PersonalProgramManager::clearPersonalProgramForPatient(QJsonObject &objPPAll)
{
    objPPAll.remove("patient_uid");         //! Удаляем пациента
    objPPAll.remove("assigned_uid");        //! Удаляем uid назначенной
    objPPAll.remove("date_begin");          //! Удаляем дату начала проведения
    objPPAll.remove("date_end");            //! Удаляем дату окончания проведения
    objPPAll.remove("active");              //! Удаляем признак активности

    //! Получаем объект ИП
    auto objPP = objPPAll["pp"].toObject();
    //! Получаем массив ДП в ИП
    auto arrDP = objPP["dp_list"].toArray();
    for (int i = 0; i < arrDP.size(); ++i)
    {
        //! Получаем объект ДП в массиве
        auto objDP = arrDP.at(i).toObject();
        objDP.remove("date_time");            //! Удаляем параметр дата и время начала ДП

        //! Получаем массив тестов в ДП
        auto arrT = objDP["test_list"].toArray();
        for (int j = 0; j < arrT.size(); ++j)
        {
            //! Получаем объект теста в массиве
            auto objT = arrT.at(j).toObject();
            objT.remove("test_uid");              //! Удаляем параметр uid теста
            //! Заменяем тест
            arrT.replace(j, objT);
        }

        //! Заменяем массив тестов
        objDP["test_list"] = arrT;

        //! Заменяем ДП
        arrDP.replace(i, objDP);
    }

    //! Заменяем массив DP
    objPP["dp_list"] = arrDP;

    //! Заменяем объект ИП
    objPPAll["pp"] = objPP;
}

QColor PersonalProgramManager::successColor(const double valSuccess)
{
    if (valSuccess >= 50)
        return QColor((50 - (static_cast<int>(valSuccess) - 50)) * 255 / 50, 255, 0);
    else
        return QColor(255, static_cast<int>(valSuccess) * 255 / 50, 0);
}

void PersonalProgramManager::on_dbConnected()
{
    assignDefaultPP();
}

QString PersonalProgramManager::minTimeBetweenDPNames(PersonalProgramDefines::MinTimeBetweenDP val)
{
    /*!
     * \brief Название значений параметра индивидуальной программы: Минимальное время между дневными программами The MinTimeBetweenDPNames enum
     */
    static QMap<PersonalProgramDefines::MinTimeBetweenDP, QString> MinTimeBetweenDPNames
    {
          std::pair<PersonalProgramDefines::MinTimeBetweenDP, QString> (PersonalProgramDefines::minOtherDay, QApplication::tr("На другой день"))
        , std::pair<PersonalProgramDefines::MinTimeBetweenDP, QString> (PersonalProgramDefines::minUndefined, QApplication::tr("Нет"))
        , std::pair<PersonalProgramDefines::MinTimeBetweenDP, QString> (PersonalProgramDefines::minOneHour, QApplication::tr("Один час"))
        , std::pair<PersonalProgramDefines::MinTimeBetweenDP, QString> (PersonalProgramDefines::minTwoHours, QApplication::tr("Два часа"))
        , std::pair<PersonalProgramDefines::MinTimeBetweenDP, QString> (PersonalProgramDefines::minFiveHours, QApplication::tr("Пять часов"))
        , std::pair<PersonalProgramDefines::MinTimeBetweenDP, QString> (PersonalProgramDefines::minTwelveHours, QApplication::tr("Двенадцать часов"))
        , std::pair<PersonalProgramDefines::MinTimeBetweenDP, QString> (PersonalProgramDefines::minDay, QApplication::tr("Одни сутки"))
        , std::pair<PersonalProgramDefines::MinTimeBetweenDP, QString> (PersonalProgramDefines::minTwoDays, QApplication::tr("Двое суток"))
        , std::pair<PersonalProgramDefines::MinTimeBetweenDP, QString> (PersonalProgramDefines::minFiveDays, QApplication::tr("Пять суток"))
    };
    return MinTimeBetweenDPNames.value(val);
}

QString PersonalProgramManager::maxTimeBetweenDPNames(PersonalProgramDefines::MaxTimeBetweenDP val)
{
    /*!
     * \brief Название значений параметра индивидуальной программы: Максимальное время между дневными программами The MaxTimeBetweenDPNames enum
     */
    static QMap<PersonalProgramDefines::MaxTimeBetweenDP, QString> MaxTimeBetweenDPNames
    {
          std::pair<PersonalProgramDefines::MaxTimeBetweenDP, QString> (PersonalProgramDefines::maxUndefined, QApplication::tr("Нет"))
        , std::pair<PersonalProgramDefines::MaxTimeBetweenDP, QString> (PersonalProgramDefines::maxOneHour, QApplication::tr("Один час"))
        , std::pair<PersonalProgramDefines::MaxTimeBetweenDP, QString> (PersonalProgramDefines::maxTwoHours, QApplication::tr("Два часа"))
        , std::pair<PersonalProgramDefines::MaxTimeBetweenDP, QString> (PersonalProgramDefines::maxFiveHours, QApplication::tr("Пять часов"))
        , std::pair<PersonalProgramDefines::MaxTimeBetweenDP, QString> (PersonalProgramDefines::maxTwelveHours, QApplication::tr("Двенадцать часов"))
        , std::pair<PersonalProgramDefines::MaxTimeBetweenDP, QString> (PersonalProgramDefines::maxOneDay, QApplication::tr("Одни сутки"))
        , std::pair<PersonalProgramDefines::MaxTimeBetweenDP, QString> (PersonalProgramDefines::maxTwoDays, QApplication::tr("Двое суток"))
        , std::pair<PersonalProgramDefines::MaxTimeBetweenDP, QString> (PersonalProgramDefines::maxThreeDays, QApplication::tr("Трое суток"))
        , std::pair<PersonalProgramDefines::MaxTimeBetweenDP, QString> (PersonalProgramDefines::maxWeek, QApplication::tr("Неделя"))
        , std::pair<PersonalProgramDefines::MaxTimeBetweenDP, QString> (PersonalProgramDefines::maxMonth, QApplication::tr("Месяц"))
    };
    return MaxTimeBetweenDPNames.value(val);
}

QString PersonalProgramManager::createDir()
{
    QString dirName = DataDefines::appCopyPath() + "personal_programs/";
    QDir dir(dirName);
    if (!dir.exists())
        dir.mkpath(dirName);
    return dirName;
}

void PersonalProgramManager::assignDefaultPP()
{
    auto groupsPP = AAnalyserBuild::getDefaultPPList();

    //! Создание папки
    QString dirName = createDir();

    foreach (auto groupPP, groupsPP)
    {
        QString namePP = "/pp.json";
        QString nameDP = "/dp.json";
        QString namePPDP = "/ppdp.json";
        if (static_cast<AAnalyserApplication*>(QApplication::instance())->languargeCode() == DataDefines::LANG_CODE_ENGUSA)
        {
            namePP = "/pp_en_US.json";
            nameDP = "/dp_en_US.json";
            namePPDP = "/ppdp_en_US.json";
        }
        auto arrPPSrc = getElements(":/pre_settings/personal_programs/" + groupPP + namePP, "pp_list");
        auto arrDPSrc = getElements(":/pre_settings/personal_programs/" + groupPP + nameDP, "dp_list");
        auto arrPPDPSrc = getElements(":/pre_settings/personal_programs/" + groupPP + namePPDP, "ppdp_list");

        auto arrPPDst = getElements(dirName + "/pp.json", "pp_list");
        auto arrDPDst = getElements(dirName + "/dp.json", "dp_list");
        auto arrPPDPDst = getElements(dirName + "/ppdp.json", "ppdp_list");

        //! Проходим по предустановленному списку ИП
        for (int i = 0; i < arrPPSrc.size(); ++i)
        {
            auto objSrc = arrPPSrc.at(i).toObject();
            //! Ищем ИП в рабочем списке. Если не нашли, то добавляем
            if (getElementsFromArray(arrPPDst, "uid", objSrc["uid"].toString()).size() == 0)
            {
                arrPPDst << objSrc;

                //! Ищем список дневных программ для этой ИП
                auto listPPDP =  getElementsFromArray(arrPPDPSrc, "pp_uid", objSrc["uid"].toString());
                foreach (auto ppdp, listPPDP)
                {
                    //! Добавляем в список ppdp
                    arrPPDPDst << ppdp;

                    //! Ищем ДП в рабочем списке. Если не нашли, то добавляем
                    if (getElementsFromArray(arrDPDst, "uid", ppdp["dp_uid"].toString()).size() == 0)
                    {
                        //! Берем из предустановленных
                        auto dpList = getElementsFromArray(arrDPSrc, "uid", ppdp["dp_uid"].toString());
                        if (dpList.size() == 1)
                            arrDPDst << dpList.at(0);
                    }
                }
            }
        }

        //! Добавить ДП, не подключенные ни к какой ИП.
        //! Проходим по предустановленному списку ДП
        for (int i = 0; i < arrDPSrc.size(); ++i)
        {
            auto objSrc = arrDPSrc.at(i).toObject();
            //! Ищем ДП в рабочем списке. Если не нашли, то добавляем
            if (getElementsFromArray(arrDPDst, "uid", objSrc["uid"].toString()).size() == 0)
                arrDPDst << objSrc;
        }

        saveElements(dirName + "/pp.json", "pp_list", arrPPDst);
        saveElements(dirName + "/dp.json", "dp_list", arrDPDst);
        saveElements(dirName + "/ppdp.json", "ppdp_list", arrPPDPDst);
    }
}

QJsonArray PersonalProgramManager::getElements(const QString &path, const QString &name)
{
    QFile file(path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray ba = file.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(ba));
        auto objs = loadDoc.object();
        file.close();
        return objs[name].toArray();
    }
    return QJsonArray();
}

void PersonalProgramManager::saveElements(const QString &path, const QString &name, QJsonArray &elements)
{
    QFile file(path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QJsonObject obj;
        obj[name] = elements;
        QJsonDocument doc(obj);
        QByteArray ba = doc.toJson();
        file.write(ba);
        file.close();
    }
}

QList<QJsonObject> PersonalProgramManager::getElementsFromArray(QJsonArray &list,
                                                                const QString &name,
                                                                const QString &value)
{
    QList<QJsonObject> retval;
    for (int i = 0; i < list.size(); ++i)
    {
        auto obj = list.at(i).toObject();
        if (obj[name].toString() == value)
            retval << obj;
    }
    return retval;
}

