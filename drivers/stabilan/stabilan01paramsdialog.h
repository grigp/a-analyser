#ifndef STABILAN01PARAMSDIALOG_H
#define STABILAN01PARAMSDIALOG_H

#include "stabilan01defines.h"
#include "deviceprotocols.h"

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

    DeviceProtocols::TensoDevices kindTenso1() const;
    void setKindTenso1(const DeviceProtocols::TensoDevices kind);

    DeviceProtocols::TensoDevices kindTenso2() const;
    void setKindTenso2(const DeviceProtocols::TensoDevices kind);

    DeviceProtocols::TensoDevices kindTenso3() const;
    void setKindTenso3(const DeviceProtocols::TensoDevices kind);

    double rkpTenso1() const;
    void setRkpTenso1(const double rkp);

    double rkpTenso2() const;
    void setRkpTenso2(const double rkp);

    double rkpTenso3() const;
    void setRkpTenso3(const double rkp);

    double pnTenso1() const;
    void setPnTenso1(const double pn);

    double pnTenso2() const;
    void setPnTenso2(const double pn);

    double pnTenso3() const;
    void setPnTenso3(const double pn);

private slots:
    void on_selectModel(int modelIdx);
    void on_selectZeroingType(int zcIdx);

private:
    Ui::Stabilan01ParamsDialog *ui;
};

#endif // STABILAN01PARAMSDIALOG_H
