#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QDir>

#include "datadefines.h"
#include "factorsdefines.h"

/*!
 * \brief Класс, осуществляющий доступ к данным DataBase class
 * Данные состоят из четырех основных таблиц: пациентов, тестов, проб, каналов.
 * Данные представляют собой файловую структуру, каждая таблица - папка, файлы в папке - записи.
 * Каждая запись, каждый файл, имеет uid, имя файла - и есть этот uid. Файл, кроме таблицы каналов, имеет структуру json, внутри которой содержатся данные записи.
 * Запись таблицы каналов представляет собой запись сигналов или данных в виде, определяемом самим каналом.
 * Таблица пациентов - папка patients. Структура файла следующая:
 * {
 *   "born": "01.03.1973",               // Дата рождения
 *   "fio": "Иванов Иван Иванович",      // ФИО
 *   "height": 176,                      // Рост
 *   "massa": 75,                        // Масса
 *   "sex": 0                            // Пол 0 - муж. 1 - жен.
 *   "data": [                           // Некоторые данные пациента с доступом по имени
 *       {
 *           "name": "data1",               // Имя данных
 *           "value": <value, QVariant>     // Значение (тип QVariant)
 *       },
 *       ...
 *   ]
 *  }
 *
 * Таблица тестов - папка tests. Структура файла следующая:
 * {
 *   "datetime": "09.07.2021 15:06:42",    // Дата и время проведения
 *   "factors": [                          // Первичные показатели со значениями. Расчитываются и обновляются при открытии теста
 *       {
 *           "channelId": "{61E814F9-CE64-46F6-B158-46961964BEE9}",   // Канал показателя
 *           "description": "Значение 1",                             // Описание
 *           "probeNum": 0,                                           // Номер пробы
 *           "uid": "{B250F6C6-673A-4CD4-976C-E26BF0294E99}",         // uid показателя
 *           "value": 0.8838859774857042                              // Значение показателя
 *       },
 *       ...
 *   ],
 *   "is_opening": false,                                     // Признак открывания теста. Устанавливается в true перед началом открывания и сбрасывается
 *                                                               после открытия. Если имеет значение true, значит в процессе открытия теста произошел сбой
 *   "metodUid": "{3664D67C-8D21-4AC8-B2DD-D5DECA43360D}",    // uid методики
 *   "params": {                                              // Параметры методики на момент проведения
 *       "direction": "up",
 *       ...
 *   },
 *   "patientUid": "{fc7dea50-ca4e-4984-a266-2b606c8f9f55}",  // uid пациента
 *   "probes": [                                              // Список проб теста
 *       {
 *           "uid": "{1e3d796a-8522-4349-9dbd-02c398181aef}"     // uid пробы в тесте
 *       }
 *   ]
 *}
 *
 * Таблица проб - папка probes. Структура файла следующая:
 * {
 *   "channels": [                                                      // Список каналов
 *       {
 *           "channelId": "{61E814F9-CE64-46F6-B158-46961964BEE9}",     // Идентификатор канала (что за канал)
 *           "uid": "{e33ad77b-bf06-4c24-87c7-73f39cd90369}"            // uid канала (файл с данными канала)
 *       },
 *       ...
 *   ],
 *   "name": "Проба 1",                                                 // Название пробы
 *   "step": 0,                                                         // Номер пробы в тесте
 *   "testUid": "{eac1f843-d74e-490d-94e0-38f7ed78dbda}"                // uid теста
 *}
 *
 */
class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();

    //!-----------------------------------------------------------------------
    //! Пациенты

    /*!
     * \brief Возвращает список пациентов в виде списка строк, содержащих UID-ы
     */
    QStringList getPatients() const;

    /*!
     * \brief Возвращает запись карточку пациента по идентификатору пациента
     * \param uid - идентификатор пациента
     * \param patient - возвращаемая карточка пациента
     * \return true, если пациент найден или false, если нет
     */
    bool getPatient(const QString &uid, DataDefines::PatientKard &patient) const;

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
    QVariant patientData(const QString &uid, const QString &name) const;

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

    //!-----------------------------------------------------------------------
    //! Сохранение теста

    /*!
     * \brief Начинает добавление записи о тесте
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


    //!-----------------------------------------------------------------------
    //! Работа с тестами

    /*!
     * \brief Возвращает список тестов в виде списка строк, содержащих UID-ы
     */
    QStringList getTests() const;

    /*!
     * \brief Возвращает запись о тесте по uid
     * \param testUid - uid теста
     * \param ti - структура данных с возвращаемым тестом
     * \return true, если удачно
     */
    bool getTestInfo(const QString &testUid, DataDefines::TestInfo &ti) const;

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
    bool getProbeInfo(const QString &probeUid, DataDefines::ProbeInfo &pi) const;

    /*!
     * \brief Возращает true, если канал имеется
     * \param probeUid - uid пробы
     * \param channelId - идентификатор канала
     */
    bool channelExists(const QString &probeUid, const QString &channelId) const;

    /*!
     * \brief Возращает true, если канал имеется
     * \param channelUid - uid канала
     */
    bool channelExists(const QString &channelUid) const;

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
    bool getChannel(const QString &probeUid, const QString &channelId, QByteArray &data) const;

    /*!
     * \brief Возвращает данные канала в виде массива байт
     * \param channelUid - uid канала
     * \param data - возвращаемые данные канала
     * \return true, если успешно
     */
    bool getChannel(const QString &channelUid, QByteArray &data) const;

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
    QString getChannelUid(const QString &probeUid, const QString &channelId) const;

    /*!
     * \brief Удаление записи о тесте
     * \param testUid - uid теста
     * \return true, если успешно
     */
    bool deleteTest(const QString &testUid) const;

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
     * \param number - номер секции в строковом виде
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
    QList<FactorsDefines::FactorValueAdvanced> getPrimaryFactors(const QString &testUid) const;

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
     * \brief Возвращает список индивидуальных норм для пациента по методике с заданными условиями проведения
     * \param patientUid - uid пациента
     * \param methodUid - uid методики
     * \param conditionUid - uid условия проведения
     * \param pnil - возвращаемые данные индивидуальной нормы
     * \return true, если нормы найдены и их удалось вернуть
     */
    bool getPersonalNorm(const QString &patientUid, const QString &methodUid, const QString &conditionUid,
                         QList<DataDefines::PersonalNormInfo> &pnil) const;

    /*!
     * \brief Возвращает список тестов для указанного пациента по указанной методике с указанными условиями проведения
     * \param patientUid - uid пациента
     * \param methodUid - uid методики
     * \param conditionUid - uid условий проведения
     */
    QStringList getTests(const QString &patientUid, const QString &methodUid, const QString &conditionUid) const;

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
     * \brief Записать измененную индивидуальную программу в БД
     * \param uidPP - uid индивидуальной программы
     * \param objPP - объект индивидуальной программы
     */
    void savePersonalProgramByUid(const QString& uidPP, const QJsonObject& objPP);

    /*!
     * \brief Возвращает список индивидуальных программ, когда либо выполнявшихся пациентом
     * В том числе и активную
     * \param patientUid - uid пациента
     */
    QJsonArray getPersonalProgramListForPatient(const QString& patientUid);

    /*!
     * \brief Удаляет все индивидуальные программы, когда-либо выполнявшиеся пациентом, в том числе и текущую
     * \param patientUid - uid пациента
     */
    void deletePersonalProgramsListForPatient(const QString& patientUid);

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

signals:
    /*!
     * \brief Извещает о подключении к БД
     */
    void connected();

    /*!
     * \brief Извещает об отключении от БД
     */
    void disconnected();

    /*!
     * \brief Извещает мир о новом тесте
     * \param testUid - uid нового теста
     */
    void newTest(const QString &testUid);

    /*!
     * \brief Извещает мир об изменении условий проведения для теста
     * \param testUid - uid теста
     */
    void changeTestCondition(const QString &testUid);

    /*!
     * \brief Сообщает миру о создании секции
     * \param channelUid - uid канала
     * \param name - название секции
     * \param number - номер секции в строковом виде
     */
    void sectionCreated(QString &channelUid, const QString &name, const QString &number);

public slots:

    /*!
     * \brief Смена базы данных
     * \param dataBaseFolder - путь к новой БД
     */
    void changeDatabase(const QString &dataBaseFolder);

    /*!
     * \brief Создание новой БД и переключение на нее
     */
    void createDatabase();

private:
    QString currentDataBase() const;
    QDir patientsDir() const;
    QDir testsDir() const;
    QDir probesDir() const;
    QDir channelsDir() const;
    QDir personalNormDir() const;
    QDir personalProgramsDir() const;
    QDir localDir(const QString &dirName) const;

    QString personalNormFileName(const QString &patientUid, const QString &methodicUid, const QString &conditionUid) const;

    QString createPatientRec(const DataDefines::PatientKard patient);

    /*!
     * \brief Читает запись из файла, как запись в таблице в виде QJsonObject
     * \param fullFileName - имя файла
     * \param rec - прочитанная запись
     * \return - true, если успешно и false, если нет
     */
    bool readTableRec(const QString &fullFileName, QJsonObject &rec) const;
    /*!
     * \brief записывает запись в файл.
     * \param fullFileName - имя файла
     * \param rec - записываемая запись
     * \return - true, если успешно и false, если нет
     */
    bool writeTableRec(const QString &fullFileName, const QJsonObject &rec) const;

    bool readSignal(const QString& fileName, QByteArray &data) const;
    bool writeSignal(const QString& fileName, QByteArray &data) const;

    bool readTestRec(const QString &fullFileName, QJsonObject &test) const;
    bool writeTestRec(const QString &fullFileName, const QJsonObject &test) const;

    void updatePatientRec(const DataDefines::PatientKard &patient);

    bool patientExists(const QString &uid) const;

    /*!
     * \brief Возвращает новое имя папки для БД
     */
    QString getNewDatabaseName();

    /*!
     * \brief Очищает папку с БД
     * \param dir - объект папки
     */
    void clearDBFolder(QDir &dir);

    void deleteAllTests();

    /*!
     * \brief Читает файл и добавляет его содержимое к байтовому массиву
     * \param fullFileName - полное имя файла (с путем)
     * \param fileName - имя файла
     * \param ba - байтовый массив
     */
    void addFileToByteArray(const QString &fullFileName, const QString &fileName, QDataStream &stream);

    QString m_dataBaseNameDef {"data"};
};

#endif // DATABASE_H
