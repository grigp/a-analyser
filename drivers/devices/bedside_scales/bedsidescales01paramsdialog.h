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

private:
    Ui::BedsideScales01ParamsDialog *ui;
};

#endif // BEDSIDESCALES01PARAMSDIALOG_H
