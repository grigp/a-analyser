#ifndef DATADEFINES_H
#define DATADEFINES_H

#include <QString>
#include <QMap>
#include <QDate>
#include <QCoreApplication>

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
    std::pair<Sex, QString> (male, QCoreApplication::tr("муж"))
  , std::pair<Sex, QString> (female, QCoreApplication::tr("жен"))
};

/*!
 * \brief Структура результатов замеров для до и после The Result struct
 */
struct Result
{
    QString uid;
    QDateTime dateTime;
    int beforePulse {0};
    int beforeDAD {0};
    double beforeKerdo {0};
    int afterPulse {0};
    int afterDAD {0};
    double afterKerdo {0};
    Result() {}
};

/*!
 * \brief Карточка человека
 */
struct PatientKard
{
    QString uid;
    QString fio;
    QDate born;
    Sex sex;
    QList<Result> results;
    PatientKard() {results.clear();}
};

/*!
 * \brief возвращает путь к папке данных для всех приложений a-analyser
 */
QString kerdoDataPath();

/*!
 * \brief возвращает путь к папке документов для всех приложений a-analyser
 */
QString kerdoDocumentsPath();

/*!
 * \brief возвращает путь к папке данных копии приложения
 */
QString appCopyPath();

/*!
 * \brief возвращает путь к папке с базой данных
 */
QString dataBasesPath();

/*!
 * \brief Возвращает uid копии приложения
 */
QString appCopyUid();

}

#endif // DATADEFINES_H
