#ifndef DATADEFINES_H
#define DATADEFINES_H

#include <QString>
#include <QDate>

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

}

#endif // DATADEFINES_H
