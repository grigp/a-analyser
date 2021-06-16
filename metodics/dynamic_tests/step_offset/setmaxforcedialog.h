#ifndef SETMAXFORCEDIALOG_H
#define SETMAXFORCEDIALOG_H

#include <QDialog>

namespace Ui {
class SetMaxForceDialog;
}

class SetMaxForceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetMaxForceDialog(QWidget *parent = nullptr);
    ~SetMaxForceDialog() override;

    void getData(const double x, const double y);

private:
    Ui::SetMaxForceDialog *ui;
};

#endif // SETMAXFORCEDIALOG_H
