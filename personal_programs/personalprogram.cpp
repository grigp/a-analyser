#include "personalprogram.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

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

    m_isActive = objPPAll["active"].toBool();
    m_dateBegin = QDate::fromString(objPPAll["date_begin"].toString(), "dd.MM.yyyy");
    m_dateEnd = QDate::fromString(objPPAll["date_end"].toString(), "dd.MM.yyyy");
    m_uidPatient = objPPAll["patient_uid"].toString();
    m_uid = objPPAll["uid"].toString();

    auto objPP = objPPAll["pp"].toObject();
    m_name = objPP["name"].toString();
    m_logoFileName = objPP["logo_file_name"].toString();
    m_minTimeBetweenDP = objPP["min_time_between_dp"].toInt();
    m_maxTimeBetweenDP = objPP["max_time_between_dp"].toInt();

    //! По дневным программам
    auto arrDP = objPP["dp_list"].toArray();
    for (int i = 0; i < arrDP.size(); ++i)
    {
        auto objDP = arrDP.at(i).toObject();

        auto itemDP = new QStandardItem("");
        itemDP->setData(objDP["uid"].toString(), DPUidRole);
        itemDP->setData(objDP["name"].toString(), DPNameRole);
        QList<QStandardItem*> items;
        items << itemDP;

        auto arrTests = objDP["test_list"].toArray();
        for (int j = 0; j < arrTests.size(); ++j)
        {
            auto objTest = arrTests.at(j).toObject();

            auto itemTest = new QStandardItem("");
            itemTest->setData(objTest["uid"].toString(), MethodUidRole);
            itemTest->setData(objTest["test_uid"].toString(), TestUidRole);
            itemTest->setData(objTest["params"].toObject(), ParamsRole);

            items << itemTest;
        }

        appendRow(items);
    }
}
