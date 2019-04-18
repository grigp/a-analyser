#ifndef PATIENTSMODEL_H
#define PATIENTSMODEL_H

#include <QObject>
#include <QStandardItemModel>

class PatientsModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit PatientsModel(QObject *parent = nullptr);

    void load();
};

#endif // PATIENTSMODEL_H
