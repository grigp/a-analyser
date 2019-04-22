#ifndef PATIENTSMODEL_H
#define PATIENTSMODEL_H

#include <QObject>
#include <QStandardItemModel>

#include "datadefines.h"

class PatientsModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit PatientsModel(QObject *parent = nullptr);

    ///< Номера колонок
    enum Columns
    {
          ColFio = 0
        , ColBorn
        , colSex
    };

    ///< Роли для доступа к данным
    enum PatientsModelRoles
    {
          PatientUidRole = Qt::UserRole + 1  ///< uid пациента. QString в итеме ФИО ColFio
        , PatientSexRole                     ///< пол пациента. int. 0 - male, 1 - female
    };

    void load();

    /*!
     * \brief Добавляет запись о пациенте
     * \param patient - запись о пациенте
     */
    void addPatient(DataDefines::PatientKard &patient);

    /*!
     * \brief Удаляет запись о пациенте
     * \param uid - uid пациента
     */
    void removePatient(const QString &uid);
};

#endif // PATIENTSMODEL_H
