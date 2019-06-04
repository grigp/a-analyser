#ifndef DATADEFINES_H
#define DATADEFINES_H

#include <QString>
#include <QMap>
#include <QDate>
#include <QStandardPaths>
#include <QApplication>

/*!
 * Набор типов данных из БД
 */
namespace DataDefines
{

/*!
 * \brief Пол пациента
 */
enum Sex
{
      male
    , female
};

static QMap<Sex, QString> SexToText{
    std::pair<Sex, QString> (male, "муж")
  , std::pair<Sex, QString> (female, "жен")
};

/*!
 * \brief Карточка пациента
 */
struct PatientKard
{
    QString uid;
    QString fio;
    QDate born;
    Sex sex;
    PatientKard() {}
};

/*!
 * \brief Структура информации о тесте TestInfo struct
 */
struct TestInfo
{
    QString uid;              ///< uid теста
    QString patientUid;       ///< uid пациента
    QString metodUid;         ///< uid методики
    QDateTime dateTime;       ///< дата и время проведения
    QStringList probes;       ///< список проб
    TestInfo() {}
};

/*!
 * \brief возвращает путь к папке данных приложения
 */
static QString appDataPath()
{
    QString path = QStandardPaths::locate(QStandardPaths::GenericDataLocation, QString(), QStandardPaths::LocateDirectory);
    return path + QApplication::applicationName() + '/';
}


static QString dataBasesPath()
{
    return appDataPath() + "databases/";
}


}

#endif // DATADEFINES_H
