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
 * \return uid пациента
 */
QString updatePatient(const DataDefines::PatientKard &patient);

/*!
 * \brief Получение именованных данных из записи пациента
 * \param uid - uid пациента
 * \param name - имя данных
 * \return данные или QVariant() при их отсутствии
 */
QVariant patientData(const QString &uid, const QString &name);

/*!
 * \brief Сохраняет именнованные данные для пациента
 * \param uid - uid пациента
 * \param name - имя данных
 * \param value - данные
 */
void patientSetData(const QString &uid, const QString &name, const QVariant value);

/*!
 * \brief Удаляет запись о пациенте
 * \param uid - uid пациента
 */
void removePatient(const QString &uid);


//! ---------------- Методики -------------------

/*!
 * \brief Возвращает полный список методик
 */
QList<MetodicDefines::MetodicInfo> getListMetodics();

/*!
 * \brief Вызывает диалог редактирования параметров методики по uid методики
 * \param metUid - uid методики
 * \param params - редактируемые параметры
 * \return true, если в диалоге нажали OK и параметры надо сохранить
 */
bool editMetodicParams(QWidget *parent, const QString &metUid);

/*!
 * \brief Вызывает диалог редактирования параметров методики по uid методики
 * \param parent - родительский виджет для вызываемого диалогового окна
 * \param metUid - uid методики
 * \param params - редактируемые параметры методики
 * \return true, если в диалоге нажали OK и параметры надо сохранить
 */
bool editMetodicParams(QWidget *parent, const QString &metUid, QJsonObject &params);

/*!
 * \brief выполнение методики
 * \param parent - родительский виджет для размещения виджета выплнения пробы
 * \param metUid - uid методики
 */
void execute(QWidget *parent, const QString &metUid);


//! ---------------- Типы методик -------------------

/*!
 * \brief Возвращает полный список типов методик
 */
QList<MetodicDefines::MetodicKindInfo> getListMetodicKinds();


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
 * \brief Сохраняет свойства теста в записи об тесте
 * \param testUid - uid теста
 * \param comment - комментарий
 * \param condition - uid условий проведения
 * \param isNorm - нормообразующее
 */
void setTestProperty(const QString &testUid,
                     const QString &comment,
                     const QString &condition,
                     const bool isNorm);

/*!
 * \brief Возвращает признак открывающегося теста
 * \param testUid - uid теста
 */
bool isTestOpening(const QString &testUid);

/*!
 * \brief Устанавливает или сбрасывает признак открывающегося теста
 * \param testUid - uid теста
 * \param isOpening - признак открывающегося теста
 */
void setTestIsOpening(const QString &testUid, const bool isOpening);

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
 * \brief Возращает true, если канал имеется
 * \param testUid - uid теста
 * \param probeNum - номер пробы в тесте
 * \param channelId - идентификатор канала в пробе
 */
bool channelExists(const QString &testUid, const int probeNum, const QString &channelId);

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
 * \brief Записывает канал в БД. Предназначен для перезаписи данных канала
 * \param probeUid - uid пробы
 * \param channelId - uid канала
 * \param data - данные канала
 * \return true, если успешно
 */
bool setChannel(const QString &probeUid, const QString &channelId, QByteArray &data);

/*!
 * \brief Возвращает channelUid, идентификатор канала в пробе
 * \param probeUid - uid пробы
 * \param channelId - идентификатор канала
 */
QString getChannelUid(const QString &probeUid, const QString &channelId);

/*!
 * \brief Возвращает список имен проб по uid теста
 * \param testUid - идентификатор теста
 */
QStringList getProbesNames(const QString& testUid);

//! ------------------- Секции сигналов -------------------

/*!
 * \brief Создает секцию сигнала
 * \param channelUid - uid канала
 * \param name - название секции
 * \param channel - номер подканала
 * \param from - начальная точка
 * \param to - конечная точка
 * \param data - байтовый массив, содержащий сигнал
 * \return true, если успешно
 */
bool createSection(QString& channelUid, QString &name, int channel, int from, int to, QByteArray &data);

/*!
 * \brief Возвращает список секций для канала
 * \param channelUid - uid канала в пробе
 * \param sections - возвращаемый список сеций
 * \return true, если успешно
 */
bool getSections(const QString& channelUid, QList<DataDefines::SectionInfo> &sections);

/*!
 * \brief Возвращает данные секции, сигнал
 * \param channelUid - uid канала в пробе
 * \param number - номер секции
 * \param data - возвращаемые данные
 * \return true, если успешно
 */
bool getSectionData(const QString &channelUid, const QString number, QByteArray &data);

/*!
 * \brief Возвращает данные секции, описание
 * \param channelUid - uid канала в пробе
 * \param number - номер секции в строковом виде
 * \param si - возвращаемые данные
 * \return true, если успешно
 */
bool getSectionData(const QString &channelUid, const QString number, DataDefines::SectionInfo &si);

/*!
 * \brief Обновляет данные секции
 * \param channelUid - uid канала в пробе
 * \param number - номер секции в строковом виде
 * \param data - новые данные секции
 * \return true, если успешно
 */
bool updateSection(const QString &channelUid, const QString &number, QByteArray &data);

/*!
 * \brief Переименование секции
 * \param channelUid - uid канала в пробе
 * \param number - номер секции
 * \param name - новое название
 * \return true, если успешно
 */
bool renameSection(const QString &channelUid, const QString &number, const QString& name);

/*!
 * \brief Удаление секции
 * \param channelUid - uid канала
 * \param number - номер секции
 * \return true, если успешно
 */
bool deleteSection(const QString &channelUid, const QString &number);

/*!
 * \brief Добавление преобразования к секции
 * \param channelUid - uid канала
 * \param number - номер секции
 * \param transformUid - uid преобразователя
 * \param params - параметры преобразователя
 * \return true, если успешно
 */
bool addTransformActionToSection(const QString &channelUid, const QString &number,
                                 const QString &transformUid, QJsonObject &params);

/*!
 * \brief Отменяет все преобразования для секции
 * \param channelUid - uid канала
 * \param number - номер секции
 * \return true, если успешно
 */
bool clearTransformActionToSection(const QString &channelUid, const QString &number);


//! ------------------- Первичные показатели -------------------

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

//! ------------------- Индивидуальные нормы -------------------

/*!
 * \brief Записывает индивидуальную норму в базу данных.
 * Записывает значение нормы для одного показателя.
 * Для методики необходимо вызвать для всех показателей отдельно
 * \param patientUid - uid пациента
 * \param methodUid - uid методики
 * \param conditionUid - uid условия проведения
 * \param factorUid - uid показателя
 * \param probeNum - номер пробы
 * \param value - значение нормы
 * \param stdDeviation - среднеквадратическое отклонение нормы
 */
void setPersonalNorm(const QString &patientUid, const QString &methodUid, const QString &conditionUid,
                     const QString &factorUid, const int probeNum, const double value, const double stdDeviation);

/*!
 * \brief Возвращает список индивидуальных норм для пациента по методике с заданными условиями проведения
 * \param patientUid - uid пациента
 * \param methodUid - uid методики
 * \param conditionUid - uid условия проведения
 * \param pnil - возвращаемые данные индивидуальной нормы
 * \return true, если нормы найдены и их удалось вернуть
 */
bool getPersonalNorm(const QString &patientUid, const QString &methodUid, const QString &conditionUid,
                     QList<DataDefines::PersonalNormInfo> &pnil);

/*!
 * \brief Возвращает true, если индивидуальная норма для пациента по методике и условий проведения существует
 * \param patientUid - uid пациента
 * \param methodUid - uid методики
 * \param conditionUid - uid условий проведения
 */
bool personalNormExists(const QString &patientUid, const QString &methodUid, const QString &conditionUid);

/*! Удаляет индивидуальную норму для пациента по методике и условий проведения
 * \brief deletePersonalNorm
 * \param patientUid - uid пациента
 * \param methodUid - uid методики
 * \param conditionUid - uid условий проведения
 */
void deletePersonalNorm(const QString &patientUid, const QString &methodUid, const QString &conditionUid);

/*!
 * \brief Воазвращает список тестов для указанного пациента по указанной методике с указанными условиями проведения
 * \param patientUid - uid пациента
 * \param methodUid - uid методики
 * \param conditionUid - uid условий проведения
 */
QStringList getTests(const QString &patientUid, const QString &methodUid, const QString &conditionUid);

/*!
 * \brief Устанавливает для теста признак включения в нормообразующие в true или false
 * \param testUid - uid теста
 * \param isNormContained - признак использования, как нормообразующее
 */
void setTestNormContained(const QString &testUid, const bool isNormContained);

/*!
 * \brief Назначает индивидуальную программу для пациента
 * \param uidPPAssigned - uid индивидуальной программы, назначенной для пациента
 * \param pp - собранная индивидуальная программа
 */
void assignPersonalProgramForPatient(const QString& uidPPAssigned, const QJsonObject& pp);

/*!
 * \brief Возвращает активную, назначенную индивидуальную программу для пациента
 * \param patientUid - uid пациента
 */
QJsonObject getActivePersonalProgramForPatient(const QString& patientUid);

/*!
 * \brief Возвращает индивидуальную программу, когда либо выполнявшуюся по uid
 * \param uidPP - uid индивидуальной программы
 */
QJsonObject getPersonalProgramByUid(const QString& uidPP);

/*!
 * \brief Возвращает список индивидуальных программ, когда либо выполнявшихся пациентом
 * В том числе и активную
 * \param patientUid - uid пациента
 */
QJsonArray getPersonalProgramListForPatient(const QString& patientUid);

/*!
 * \brief Возвращает полный список индивидуальных программ, когда либо выполнявшихся
 */
QJsonArray getPersonalProgramList();

/*!
 * \brief Удаляет индивидуальную программу, назначенную для пациента
 * При удалении по индивидуальной программе не должно быть проведено тестов.
 * Функция этого не прооверяет, но логически это неверно
 * \param ppUid - uid индивидуальной программы, назначенной для пациента
 */
void deletePersonalProgramForPatient(const QString& ppUid);

/*!
 * \brief Отменяет индивидуальную программу для пациента
 * При отмене по индивидуальной программе должен быть проведен хотя бы один тест.
 * Функция этого не прооверяет, но логически это неверно
 * \param ppUid - uid индивидуальной программы, назначенной для пациента
 */
void deactivatePersonalProgramForPatient(const QString& ppUid);


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
