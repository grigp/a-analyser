#ifndef AMEDPLATFORM01PARAMSDIALOG_H
#define AMEDPLATFORM01PARAMSDIALOG_H

#include <QDialog>

#include "deviceprotocols.h"

namespace Ui {
class AMedPlatform01ParamsDialog;
}

/*!
 * \brief Класс диалога редактирования параметров драйвера стабилоплатформы А-Мед The AMedPlatform01ParamsDialog class
 */
class AMedPlatform01ParamsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AMedPlatform01ParamsDialog(QWidget *parent = nullptr);
    ~AMedPlatform01ParamsDialog();

    int frequency() const;
    void setFrequency(const int freq);

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


private:
    Ui::AMedPlatform01ParamsDialog *ui;
};

#endif // AMEDPLATFORM01PARAMSDIALOG_H
