#ifndef MWSTICKPARAMSDIALOG_H
#define MWSTICKPARAMSDIALOG_H

#include <QDialog>

namespace Ui {
class MWStickParamsDialog;
}

/*!
 * \brief Класс диалога редактирования параметров соревновательной палки The MWStickParamsDialog class
 */
class MWStickParamsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MWStickParamsDialog(QWidget *parent = nullptr);
    ~MWStickParamsDialog() override;

    int channel() const;
    void setChannel(const int chan);

    double rkp() const;
    void setRkp(const double rkp);

    int pn() const;
    void setPn(const int pn);


private:
    Ui::MWStickParamsDialog *ui;
};

#endif // MWSTICKPARAMSDIALOG_H
