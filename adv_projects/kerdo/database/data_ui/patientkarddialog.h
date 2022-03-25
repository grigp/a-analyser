#ifndef PATIENTKARDDIALOG_H
#define PATIENTKARDDIALOG_H

#include <QDialog>

#include "datadefines.h"

namespace Ui {
class PatientKardDialog;
}

class PatientKardDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PatientKardDialog(QWidget *parent = 0);
    ~PatientKardDialog();

    QString fio() const;
    void setFio(const QString &fio);

    QDate born() const;
    void setBorn(const QDate &born);

    DataDefines::Sex sex() const;
    void setSex(const DataDefines::Sex &sex);


private:
    Ui::PatientKardDialog *ui;
};

#endif // PATIENTKARDDIALOG_H
