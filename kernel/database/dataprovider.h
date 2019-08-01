#ifndef DATAPROVIDER_H
#define DATAPROVIDER_H

#include <QStringList>

#include "datadefines.h"
#include "metodicdefines.h"
#include "factorsdefines.h"

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

//! Сохранение теста

/*!
 * \brief Добавляет запись о тесте
 * \param patientUid - uid пациента
 * \param metodUid - uid методики
 * \return uid созданного теста
 */
QString addTestStart(const QString &patientUid, const QString &metodUid);

/*!
 * \brief Завершает добавление записи о тесте
 * \param testUid - uid добавленного теста
 */
void addTestFinish(const QString &testUid);

/*!
 * \brief Добавляет запись о пробе
 * \param testUid - uid теста
 * \param step - номер шага
 * \return uid пробы
 */
QString addProbe(const QString &testUid, const int step, const QString &name);

/*!
 * \brief Добавляет сигнал
 * \param probeUid - uid пробы
 * \param channelUid - uid канала
 * \param data - Данные сигнала
 */
void addSignal(const QString &probeUid, const QString &channelUid, const QByteArray &data);


//! Работа с тестами

/*!
 * \brief Возвращает список тестов в виде списка строк, содержащих UID-ы
 */
QStringList getTests();

/*!
 * \brief Возвращает запись о тесте по uid
 * \param testUid - uid теста
 * \param ti - структура данных с возвращаемым тестом
 * \return true, если удачно
 */
bool getTestInfo(const QString &testUid, DataDefines::TestInfo &ti);

/*!
 * \brief Возвращает запись о пробе по uid
 * \param probeUid - uid пробы
 * \param pi - структура данных с возвращаемой пробой
 * \return true, если удачно
 */
bool getProbeInfo(const QString &probeUid, DataDefines::ProbeInfo &pi);

/*!
 * \brief Возращает true, если канал имеется
 * \param probeUid - uid пробы
 * \param channelId - идентификатор канала
 */
bool channelExists(const QString &probeUid, const QString &channelId);

/*!
 * \brief Возращает true, если канал имеется
 * \param channelUid - uid канала
 */
bool channelExists(const QString &channelUid);

/*!
 * \brief Возвращает данные канала в виде массива байт
 * \param probeUid - uid пробы
 * \param channelId - идентификатор канала
 * \param data - возвращаемые данные канала
 * \return true, если успешно
 */
bool getChannel(const QString &probeUid, const QString &channelId, QByteArray &data);

/*!
 * \brief Возвращает данные канала в виде массива байт
 * \param channelUid - uid канала
 * \param data - возвращаемые данные канала
 * \return true, если успешно
 */
bool getChannel(const QString &channelUid, QByteArray &data);


/*!
 * \brief Добавляет в запись о тесте запись о первичном показателе
 * \param testUid - uid теста
 * \param uid - uid показателя
 * \param value - значение показателя
 * \param probeNum - номер пробы
 * \param channelId - идентификатор канала
 * \param description - описание показателя
 */
void addPrimaryFactor(const QString &testUid,
                      const QString &uid,
                      const double value,
                      const int probeNum,
                      const QString &channelId,
                      const QString &description);

/*!
 * \brief Возвращает список первичных показателей для теста по uid
 */
QList<FactorsDefines::FactorValueAdvanced> getPrimaryFactors(const QString &testUid);

}

#endif // DATAPROVIDER_H
