#ifndef RUNNINGMODEDIALOG_H
#define RUNNINGMODEDIALOG_H

#include <QDialog>

#include "basedefines.h"

namespace Ui {
class RunningModeDialog;
}

/*!
 * \brief Класс диалога выбора режима запуска тестов: автоматический или с оператором The RunningModeDialog class
 */
class RunningModeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RunningModeDialog(QWidget *parent = nullptr);
    ~RunningModeDialog();

    BaseDefines::RunningMode runningMode() const {return m_runningMode;}
    void setRunningMode(const BaseDefines::RunningMode rm);

private slots:
    void on_setOperatorMode();
    void on_setAutoMode();

private:
    Ui::RunningModeDialog *ui;

    BaseDefines::RunningMode m_runningMode {BaseDefines::rmOperator};
};

#endif // RUNNINGMODEDIALOG_H
