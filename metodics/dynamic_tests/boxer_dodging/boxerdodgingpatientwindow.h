#ifndef BOXERDODGINGPATIENTWINDOW_H
#define BOXERDODGINGPATIENTWINDOW_H

#include <QDialog>

namespace Ui {
class BoxerDodgingPatientWindow;
}

/*!
 * \brief Класс окна пациентов тренажера с уклонением для боксеров BoxerDodgingPatientWindow class
 */
class BoxerDodgingPatientWindow : public QDialog
{
    Q_OBJECT

public:
    explicit BoxerDodgingPatientWindow(QWidget *parent = nullptr);
    ~BoxerDodgingPatientWindow();

private:
    Ui::BoxerDodgingPatientWindow *ui;
};

#endif // BOXERDODGINGPATIENTWINDOW_H
