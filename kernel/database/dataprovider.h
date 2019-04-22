#ifndef DATAPROVIDER_H
#define DATAPROVIDER_H

#include <QStringList>

#include "datadefines.h"

/*!
 * Набор функций для доступа к данным
 */
namespace DataProvider
{

/*!
 * \brief Возвращает список пациентов в виде списка строк, содержащих UID-ы
 */
QStringList getPatients();

/*!
 * \brief Возвращает запись карточку пациента по идентификатору пациента
 * \param uid - идентификатор пациента
 * \param patient - возвращаемая карточка пациента
 * \return true, если пациент найден или false, если нет
 */
bool getPatient(const QString &uid, DataDefines::PatientKard &patient);

/*!
 * \brief Обновляет данные о пациенте.
 * Если карточка имеется, то обновляет в ней данные,
 * если нет, то добавляет пациента
 * \param patient - карточка пациента
 */
void updatePatient(const DataDefines::PatientKard &patient);

/*!
 * \brief Удаляет запись о пациенте
 * \param uid - uid пациента
 */
void removePatient(const QString &uid);
}

#endif // DATAPROVIDER_H
