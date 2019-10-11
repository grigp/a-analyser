#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QDir>

#include "datadefines.h"
#include "factorsdefines.h"

/*!
 * \brief Класс, осуществляющий доступ к данным DataBase class
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
     */
    void updatePatient(const DataDefines::PatientKard &patient);

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
     * \brief Удаление записи о тесте
     * \param testUid - uid теста
     * \return true, если успешно
     */
    bool deleteTest(const QString &testUid) const;

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
     * \brief Воазвращает список тестов для указанного пациента по указанной методике с указанными условиями проведения
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
    QDir localDir(const QString &dirName) const;

    QString personalNormFileName(const QString &patientUid, const QString &methodicUid, const QString &conditionUid) const;

    void createPatientRec(const DataDefines::PatientKard patient);

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
