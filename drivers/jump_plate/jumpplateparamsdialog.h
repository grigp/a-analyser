#ifndef JUMPPLATEPARAMSDIALOG_H
#define JUMPPLATEPARAMSDIALOG_H

#include <QDialog>

namespace Ui {
class JumpPlateParamsDialog;
}

class JumpPlateParamsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit JumpPlateParamsDialog(QWidget *parent = 0);
    ~JumpPlateParamsDialog();

private:
    Ui::JumpPlateParamsDialog *ui;
};

#endif // JUMPPLATEPARAMSDIALOG_H
