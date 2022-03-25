#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QDir>

#include "datadefines.h"

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

    //!-----------------------------------------------------------------------
    //! Разное

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
    QDir localDir(const QString &dirName) const;

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
