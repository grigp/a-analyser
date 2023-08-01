#ifndef PATIENTSMODEL_H
#define PATIENTSMODEL_H

#include <QObject>
#include <QStandardItemModel>

#include "datadefines.h"

/*!
 * \brief Класс модели списка пациентов PatientsModel class
 */
class PatientsModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit PatientsModel(QObject *parent = nullptr);

    void load();

    /*!
     * \brief Добавляет запись о пациенте
     * \param patient - запись о пациенте
     */
    QString addPatient(DataDefines::PatientKard &patient);

    /*!
     * \brief Удаляет запись о пациенте
     * \param uid - uid пациента
     */
    void removePatient(const QString &uid);

private:
    void addPatientInModel(DataDefines::PatientKard &patient);
};

#endif // PATIENTSMODEL_H
