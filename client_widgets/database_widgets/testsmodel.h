#ifndef TESTSMODEL_H
#define TESTSMODEL_H

#include <QObject>
#include <QStandardItemModel>

class TestsModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit TestsModel(QObject *parent = nullptr);

    ///< Номера колонок
    enum Columns
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

    void load();

private slots:
    void onNewTest(const QString &testUid);
    void onRemoveTest(const QString &testUid);

private:
    void addTest(const QString &testUid);

};

#endif // TESTSMODEL_H
