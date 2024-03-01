#include "personalprogram.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>

#include "personalprogramdefines.h"

PersonalProgram::PersonalProgram(QObject *parent)
    : QStandardItemModel (parent)
{

}

void PersonalProgram::load(const QString &fileName)
{
    QJsonObject objPPAll;

    //! Чтение файла
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray ba = file.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(ba));
        objPPAll = loadDoc.object();
        file.close();
    }

    load(objPPAll);
}

void PersonalProgram::load(const QJsonObject &objPPAll)
{
    m_isActive = objPPAll["active"].toBool();
    m_dateBegin = QDate::fromString(objPPAll["date_begin"].toString(), "dd.MM.yyyy");
    m_dateEnd = QDate::fromString(objPPAll["date_end"].toString(), "dd.MM.yyyy");
    m_uidPatient = objPPAll["patient_uid"].toString();
    m_uid = objPPAll["pp_uid"].toString();
    m_uidAssigned = objPPAll["assigned_uid"].toString("");

    auto objPP = objPPAll["pp"].toObject();
    m_name = objPP["name"].toString();
    m_logoFileName = objPP["logo_file_name"].toString();
    m_minTimeBetweenDP = objPP["min_time_between_dp"].toInt();
    m_maxTimeBetweenDP = objPP["max_time_between_dp"].toInt();

    //! По дневным программам
    auto arrDP = objPP["dp_list"].toArray();
    for (int i = 0; i < arrDP.size(); ++i)
    {
        //! Объект дневной программы
        auto objDP = arrDP.at(i).toObject();
        addDailyProgram(objDP);
    }
}

QJsonObject PersonalProgram::save()
{
    QJsonObject retval;

    retval["patient_uid"] = m_uidPatient;
    retval["pp_uid"] = m_uid;
    retval["assigned_uid"] = m_uidAssigned;
    retval["active"] = m_isActive;
    retval["date_begin"] = m_dateBegin.toString("dd.MM.yyyy");
    if (m_dateEnd != QDate())
        retval["date_end"] = m_dateEnd.toString("dd.MM.yyyy");

    QJsonObject objPP;
    objPP["logo_file_name"] = m_logoFileName;
    objPP["min_time_between_dp"] = m_minTimeBetweenDP;
    objPP["max_time_between_dp"] = m_maxTimeBetweenDP;
    objPP["name"] = m_name;
    objPP["uid"] = m_uid;

    QJsonArray arrDP;
    for (int i = 0; i < rowCount(); ++i)
    {
        auto idxRootDP = index(i, 0);
        QJsonObject objDP;
        objDP["uid"] = idxRootDP.data(PersonalProgramDefines::PersonalProgram::DPUidRole).toString();
        objDP["name"] = idxRootDP.data(PersonalProgramDefines::PersonalProgram::DPNameRole).toString();
        QString dt = idxRootDP.data(PersonalProgramDefines::PersonalProgram::DPDateTimeRole).toString();
        if (dt != "")
            objDP["date_time"] = dt;

        QJsonArray arrTests;
        for (int j = 1; j < columnCount(); ++j)
        {
            auto idxTest = index(i, j);
            auto uidMethod = idxTest.data(PersonalProgramDefines::PersonalProgram::MethodUidRole).toString();

            if (uidMethod != "")
            {
                QJsonObject objTest;
                objTest["uid"] = uidMethod;
                objTest["params"] = idxTest.data(PersonalProgramDefines::PersonalProgram::ParamsRole).toJsonObject();
                auto uidTest = idxTest.data(PersonalProgramDefines::PersonalProgram::TestUidRole).toString();
                if (uidTest != "")
                    objTest["test_uid"] = uidTest;

                arrTests << objTest;
            }
        }
        objDP["test_list"] = arrTests;
        arrDP << objDP;
    }

    objPP["dp_list"] = arrDP;
    retval["pp"] = objPP;

    return retval;
}

void PersonalProgram::addDailyProgram(const QJsonObject &objDP)
{
    //! Заголовок дневной программы
    auto dt = objDP["date_time"].toString();

    QString dpTitle = tr("Занятие") + " " + QString::number(rowCount() + 1);//(i + 1);
    if (dt != "")
        dpTitle += ("\n" + dt);

    //! Создаем итем дневной программы
    auto itemDP = new QStandardItem(dpTitle);
    itemDP->setData(objDP["uid"].toString(), PersonalProgramDefines::PersonalProgram::DPUidRole);
    itemDP->setData(objDP["name"].toString(), PersonalProgramDefines::PersonalProgram::DPNameRole);
    itemDP->setData(dt, PersonalProgramDefines::PersonalProgram::DPDateTimeRole);
    itemDP->setEditable(false);
    QList<QStandardItem*> items;
    items << itemDP;

    //! Тесты в дневной программе
    auto arrTests = objDP["test_list"].toArray();
    for (int j = 0; j < arrTests.size(); ++j)
    {
        //! Объект теста
        auto objTest = arrTests.at(j).toObject();

        //! Итем теста
        auto itemTest = new QStandardItem("");
        itemTest->setData(objTest["uid"].toString(), PersonalProgramDefines::PersonalProgram::MethodUidRole);
        itemTest->setData(objTest["test_uid"].toString(), PersonalProgramDefines::PersonalProgram::TestUidRole);
        itemTest->setData(objTest["params"].toObject(), PersonalProgramDefines::PersonalProgram::ParamsRole);
        itemTest->setEditable(false);

        items << itemTest;
    }

    //! Строка дневной программы
    appendRow(items);
}

void PersonalProgram::doubleDailyProgram(const int number)
{
    //! Строка существует
    if (0 <= number && rowCount() > number)
    {
        //! Создаем итем дневной программы
        auto itemDP = new QStandardItem(index(number, 0).data().toString());
        auto uidDP = index(number, 0).data(PersonalProgramDefines::PersonalProgram::DPUidRole).toString();
        auto nameDP = index(number, 0).data(PersonalProgramDefines::PersonalProgram::DPNameRole).toString();
        auto dt = index(number, 0).data(PersonalProgramDefines::PersonalProgram::DPDateTimeRole).toString();

        itemDP->setData(uidDP, PersonalProgramDefines::PersonalProgram::DPUidRole);
        itemDP->setData(nameDP, PersonalProgramDefines::PersonalProgram::DPNameRole);
        itemDP->setData(dt, PersonalProgramDefines::PersonalProgram::DPDateTimeRole);
        itemDP->setEditable(false);
        QList<QStandardItem*> items;
        items << itemDP;

        //! Тесты в дневной программе
        for (int j = 1; j < columnCount(); ++j)
        {
            //! Индекс элемента
            auto idx = index(number, j);

            //! Итем теста
            auto itemTest = new QStandardItem("");
            auto uidMet = idx.data(PersonalProgramDefines::PersonalProgram::MethodUidRole).toString();
            auto uidTest = idx.data(PersonalProgramDefines::PersonalProgram::TestUidRole).toString();
            auto params = idx.data(PersonalProgramDefines::PersonalProgram::ParamsRole);

            itemTest->setData(uidMet, PersonalProgramDefines::PersonalProgram::MethodUidRole);
            itemTest->setData(uidTest, PersonalProgramDefines::PersonalProgram::TestUidRole);
            itemTest->setData(params, PersonalProgramDefines::PersonalProgram::ParamsRole);
            itemTest->setEditable(false);

            items << itemTest;
        }

        //! Строка дневной программы
        insertRow(number + 1, items);
    }
}

void PersonalProgram::addTest(int numDP, QString &metUid, QJsonObject &params)
{
    //! Кол-во тестов в дневной программе
    int tc = 0;
    for (int i = 1; i < columnCount(); ++i)
    {
        auto idx = index(numDP, i);
        if (idx.isValid() && idx.data(PersonalProgramDefines::PersonalProgram::MethodUidRole).toString() != "")
            tc = i;
        else
            break;
    }

    //! Итем теста
    auto itemTest = new QStandardItem("");
    itemTest->setData(metUid, PersonalProgramDefines::PersonalProgram::MethodUidRole);
    itemTest->setData(params, PersonalProgramDefines::PersonalProgram::ParamsRole);

    setItem(numDP, tc + 1, itemTest);
}

bool PersonalProgram::isTestByPPExists(QJsonObject &pp)
{
    auto objPp = pp["pp"].toObject();
    auto arrDpList = objPp["dp_list"].toArray();
    if (arrDpList.size() > 0)
    {
        auto objDp = arrDpList.at(0).toObject();
        auto arrTList = objDp["test_list"].toArray();
        if (arrTList.size() > 0)
        {
            auto objT = arrTList.at(0).toObject();
            auto testUid = objT["test_uid"].toString("");
            return testUid != "";
        }
    }
    return false;
}

bool PersonalProgram::isTestByPPExists()
{
//TODO: написатьs
}
