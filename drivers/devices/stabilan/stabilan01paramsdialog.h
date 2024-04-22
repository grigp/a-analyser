#ifndef STABILAN01PARAMSDIALOG_H
#define STABILAN01PARAMSDIALOG_H

#include "stabilan01defines.h"
#include "deviceprotocols.h"

#include <QDialog>

namespace Ui {
class Stabilan01ParamsDialog;
}

/*!
 * \brief Класс диалога редактирования параметров стабилоанализатора Стабилан-01 The Stabilan01ParamsDialog class
 */
class Stabilan01ParamsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit Stabilan01ParamsDialog(QWidget *parent = nullptr);
    ~Stabilan01ParamsDialog() override;

    Stabilan01Defines::Model model() const;
    void setModel(const Stabilan01Defines::Model mdl);

    Stabilan01Defines::ZeroingType zeroingType() const;
    void setZeroingType(const Stabilan01Defines::ZeroingType zt);

    Stabilan01Defines::SKGSource skgSource() const;
    void setSKGSource(const Stabilan01Defines::SKGSource src);

    void setRecording(const QMap<QString,  bool>& recMap);
    QMap<QString, bool> getRecording() const;

    DeviceProtocols::TensoDevice kindTenso1() const;
    void setKindTenso1(const DeviceProtocols::TensoDevice kind);

    DeviceProtocols::TensoDevice kindTenso2() const;
    void setKindTenso2(const DeviceProtocols::TensoDevice kind);

    DeviceProtocols::TensoDevice kindTenso3() const;
    void setKindTenso3(const DeviceProtocols::TensoDevice kind);

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

    double maxDifferent() const;
    void setMaxDifferent(const double val);

    double minWeight() const;
    void setMinWeight(const double val);

private slots:
    void on_selectModel(int modelIdx);
    void on_selectZeroingType(int zcIdx);

private:
    Ui::Stabilan01ParamsDialog *ui;
};

#endif // STABILAN01PARAMSDIALOG_H
