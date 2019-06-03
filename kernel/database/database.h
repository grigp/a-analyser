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
    QStringList getPatients();

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
     * \brief Добавляет запись о тесте
     * \param patientUid - uid пациента
     * \param metodUid - uid методики
     * \return uid созданного теста
     */
    QString addTest(const QString &patientUid, const QString &metodUid);

    /*!
     * \brief Добавляет запись о пробе
     * \param testUid - uid теста
     * \return uid пробы
     */
    QString addProbe(const QString &testUid);

    /*!
     * \brief Добавляет сигнал
     * \param probeUid - uid пробы
     * \param channelUid - uid канала
     * \param data - Данные сигнала
     */
    void addSignal(const QString &probeUid, const QString &channelUid, const QByteArray &data);

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

    bool readPatientRec(const QString &fullFileName, QJsonObject &patient) const;
    bool writePatientRec(const QString &fullFileName, QJsonObject &patient) const;

    void updatePatientRec(const DataDefines::PatientKard &patient);

    bool patientExists(const QString &uid) const;

    QString m_dataBaseName = "data";
};

#endif // DATABASE_H
