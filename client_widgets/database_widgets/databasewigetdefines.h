#ifndef DATABASEWIGETDEFINES_H
#define DATABASEWIGETDEFINES_H

#include <QObject>

namespace DatabaseWidgetDefines
{

namespace PatientsModel
{

///< Номера колонок
enum PatientsModelColumns
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
    , PatientPPUidRole                   ///< uid назначенной пациенту индивидуальной программы. QString в итеме ФИО ColFio "" - не назначена
    , PatientTableUserRole = PatientUidRole + 100  ///< Начальная роль для ролей пользователя, связанных с таблицей пациентов
};


}

namespace MetodicsModel
{

///< Номера колонок
enum MetodicsModelColumns
{
      ColName = 0
};

enum MetodicsModelRoles
{
      TemplateUidRole = Qt::UserRole + 1  ///< uid шаблона методики. QString в итеме названия ColName
    , MetodicUidRole                      ///< uid методики. QString в итеме названия ColName
    , MetodicParamsRole                   ///< параметры методики. QJsonObject в итеме названия ColName
};

}


namespace TestsModel
{

///< Номера колонок
enum TestsModelColumns
{
      ColPatient = 0
    , ColMetodic
    , ColDateTime
};

///< Роли для доступа к данным
enum TestsModelRoles
{
      TestUidRole = Qt::UserRole + 1     ///< uid теста. QString в итеме ColPatient
    , PatientUidRole                     ///< uid пациента. QString в итеме ColPatient
    , MetodicUidRole                     ///< uid методики. QString в итеме ColMetodic
};

}

};

#endif // DATABASEWIGETDEFINES_H
