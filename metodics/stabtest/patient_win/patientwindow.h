#ifndef PATIENTWINDOW_H
#define PATIENTWINDOW_H

#include <QDialog>

namespace Ui {
class PatientWindow;
}

/*!
 * \brief Базовый класс окон пациентов PatientWindow class
 */
class PatientWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PatientWindow(QWidget *parent = 0);
    ~PatientWindow();

    virtual void setDiap(const int diap) = 0;
    virtual void setMarker(const double x, const double y) = 0;

    virtual void run() = 0;
    virtual void stop() = 0;

    /*!
     * \brief Возвращает результат работы окна пациента
     */
    virtual QVariant result() = 0;

private:
    Ui::PatientWindow *ui;
};

#endif // PATIENTWINDOW_H
