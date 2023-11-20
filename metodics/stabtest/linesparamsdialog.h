#ifndef LINESPARAMSDIALOG_H
#define LINESPARAMSDIALOG_H

#include <QDialog>
#include "basedefines.h"

namespace Ui {
class LinesParamsDialog;
}

/*!
 * \brief Диалог параметров оптокинетической стимуляции The LinesParamsDialog class
 */
class LinesParamsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LinesParamsDialog(QWidget *parent = nullptr);
    ~LinesParamsDialog();

    BaseDefines::Directions direction() const;
    void setDirection(const BaseDefines::Directions dir);

    int width() const;
    void setWidth(const int w);

    int speed() const;
    void setSpeed(const int spd);

    int dutyCycle() const;
    void setDutyCycle(const int dc);

    QColor color() const;
    void setColor(const QColor col);

private slots:
    void on_selectColor();

private:
    Ui::LinesParamsDialog *ui;


};

#endif // LINESPARAMSDIALOG_H
