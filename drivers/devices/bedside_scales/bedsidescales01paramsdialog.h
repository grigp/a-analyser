#ifndef BEDSIDESCALES01PARAMSDIALOG_H
#define BEDSIDESCALES01PARAMSDIALOG_H

#include <QDialog>

namespace Ui {
class BedsideScales01ParamsDialog;
}


/*!
 * \brief Класс диалога редактирования параметров драйвера прикроватных весов The BedsideScales01ParamsDialog class
 */
class BedsideScales01ParamsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BedsideScales01ParamsDialog(QWidget *parent = nullptr);
    ~BedsideScales01ParamsDialog() override;

    double rkpSensor1() const;
    void setRkpSensor1(const double rkp);

    double rkpSensor2() const;
    void setRkpSensor2(const double rkp);

    double rkpSensor3() const;
    void setRkpSensor3(const double rkp);

    double rkpSensor4() const;
    void setRkpSensor4(const double rkp);

    int pn() const;
    void setPn(const int pn);

private:
    Ui::BedsideScales01ParamsDialog *ui;

};

#endif // BEDSIDESCALES01PARAMSDIALOG_H
