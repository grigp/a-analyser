#ifndef DATAPROVIDER_H
#define DATAPROVIDER_H

#include <QStringList>

#include "datadefines.h"
#include "metodicdefines.h"

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
 */
void updatePatient(const DataDefines::PatientKard &patient);

/*!
 * \brief Удаляет запись о пациенте
 * \param uid - uid пациента
 */
void removePatient(const QString &uid);


//! ---------------- Методики -------------------

/*!
 * \brief Возвращает полный список методик
 */
QList<MetodicDefines::MetodicInfo> getListMetodisc();

/*!
 * \brief Вызывает диалог редактирования параметров методики по uid методики
 * \param metUid - uid методики
 * \param params - редактируемые параметры
 * \return true, если в диалоге нажали OK и параметры надо сохранить
 */
bool editMetodicParams(QWidget *parent, const QString &metUid);

/*!
 * \brief выполнение методики
 * \param parent - родительский виджет для размещения виджета выплнения пробы
 * \param metUid - uid методики
 */
void execute(QWidget *parent, const QString &metUid);

//! ---------------- Тесты -------------------


}

#endif // DATAPROVIDER_H
