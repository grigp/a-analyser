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

    int platformsCount() const;
    void setPlatformsCount(const int count);

private:
    Ui::JumpPlateParamsDialog *ui;
};

#endif // JUMPPLATEPARAMSDIALOG_H
