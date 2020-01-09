#ifndef STABILAN01PARAMSDIALOG_H
#define STABILAN01PARAMSDIALOG_H

#include "stabilan01defines.h"

#include <QDialog>

namespace Ui {
class Stabilan01ParamsDialog;
}

class Stabilan01ParamsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit Stabilan01ParamsDialog(QWidget *parent = 0);
    ~Stabilan01ParamsDialog();

    Stabilan01Defines::Model model() const;
    void setModel(const Stabilan01Defines::Model mdl);

    Stabilan01Defines::ZeroingType zeroingType() const;
    void setZeroingType(const Stabilan01Defines::ZeroingType zt);

    void setRecording(const QMap<QString,  bool>& recMap);
    QMap<QString, bool> getRecording() const;

private:
    Ui::Stabilan01ParamsDialog *ui;
};

#endif // STABILAN01PARAMSDIALOG_H
