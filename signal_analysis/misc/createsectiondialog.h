#ifndef CREATESECTIONDIALOG_H
#define CREATESECTIONDIALOG_H

#include <QDialog>

namespace Ui {
class CreateSectionDialog;
}

class SignalAccess;

class CreateSectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateSectionDialog(QWidget *parent = nullptr);
    ~CreateSectionDialog() override;

    QString sectionName() const;
    void setSectionName(const QString name);

    void assignSignal(SignalAccess* signal, const int def = 0);
    int channel() const;

public slots:
    void accept() override;

private:
    Ui::CreateSectionDialog *ui;
};

#endif // CREATESECTIONDIALOG_H
