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

signals:
    /*!
     * \brief Извещает о подключении к БД
     */
    void connected();

    /*!
     * \brief Извещает об отключении от БД
     */
    void disconnected();

public slots:

private:
    QString currentDataBase() const;
    QDir patientsDir() const;

    void createPatientRec(const DataDefines::PatientKard patient);

    bool readPatientRec(const QString &fullFileName, DataDefines::PatientKard &patient);

    void updatePatientRec(const DataDefines::PatientKard &patient);

    bool patientExists(const QString &uid) const;

    QString m_dataBaseName = "data";
};

#endif // DATABASE_H
