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

    void load();

    /*!
     * \brief Добавляет запись о пациенте
     * \param patient - запись о пациенте
     */
    void addPatient(DataDefines::PatientKard &patient);
};

#endif // PATIENTSMODEL_H
