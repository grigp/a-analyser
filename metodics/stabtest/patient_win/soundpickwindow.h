#ifndef SOUNDPICKWINDOW_H
#define SOUNDPICKWINDOW_H

#include <QDialog>
#include "patientwindow.h"

namespace Ui {
class SoundPickWindow;
}

/*!
 * \brief Форма пациента "Звуковые сигалы" SoundPickWindow class
 */
class SoundPickWindow : public PatientWindow
{
    Q_OBJECT

public:
    explicit SoundPickWindow(QWidget *parent = 0);
    ~SoundPickWindow();

    void setStabData(const double x, const double y) override;

    void run() override;
    void stop() override;

private:
    Ui::SoundPickWindow *ui;
};

#endif // SOUNDPICKWINDOW_H
