#ifndef TESTSMODEL_H
#define TESTSMODEL_H

#include <QObject>
#include <QStandardItemModel>

class TestsModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit TestsModel(QObject *parent = nullptr);


    void load();

public slots:
    void on_updatePatientData(const QString &patientUid);

private slots:
    void onNewTest(const QString &testUid);
    void onRemoveTest(const QString &testUid);

private:
    void addTest(const QString &testUid);

};

#endif // TESTSMODEL_H
