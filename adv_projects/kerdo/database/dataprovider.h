#ifndef DATAPROVIDER_H
#define DATAPROVIDER_H

#include <QStringList>

#include "datadefines.h"

/*!
 * Набор функций для доступа к данным
 */
namespace DataProvider
{

//! ---------------- Пациенты -------------------

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
 * \return uid пациента
 */
QString updatePatient(const DataDefines::PatientKard &patient);

/*!
 * \brief Удаляет запись о пациенте
 * \param uid - uid пациента
 */
void removePatient(const QString &uid);

//!-----------------------------------------------------------------------
//! Тесты

/*!
 * \brief Добавление теста с нулевыми значениями результатов для пациента
 * \param uidPatient - uid пациента
 * \return uid теста
 */
QString addTest(const QString &uidPatient);

/*!
 * \brief Возвращает список тестов для пациента
 * \param uidPatient - uid пациента
 * \return список uid тестов
 */
QStringList getTests(const QString &uidPatient);

/*!
 * \brief Возвращает значения результатов теста для пациента
 * \param uidPatient - uid пациента
 * \param uidTest - uid теста
 * \param test - значения результатов теста
 * \return true, если удачно и false, если нет
 */
bool getTest(const QString &uidPatient, const QString &uidTest, DataDefines::Result &test);

/*!
 * \brief Записывает результаты теста для пациента
 * \param uidPatient - uid пациента
 * \param uidTest - uid теста
 * \param test - результаты теста
 */
void setTestResult(const QString &uidPatient, const QString &uidTest, const DataDefines::Result &test);

//! ------------------- Общие и сервисные операции -------------------
/*!
 * \brief Очищает всю БД
 */
void clear();

/*!
 * \brief Удаляет все тесты со всеми данными
 */
void deleteTests();

/*!
 * \brief Экспорт БД в файл
 * \param fileName - имя файла
 */
void exportBD(const QString &fileName);

/*!
 * \brief Импорт БД из файла
 * \param fileName - имя файла
 */
void importBD(const QString &fileName);


}

#endif // DATAPROVIDER_H
