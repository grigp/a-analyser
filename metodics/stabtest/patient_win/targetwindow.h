#ifndef TARGETWINDOW_H
#define TARGETWINDOW_H

#include <QDialog>

#include "patientwindow.h"

namespace Ui {
class TargetWindow;
}

class TargetWindow : public PatientWindow
{
    Q_OBJECT

public:
    explicit TargetWindow(QWidget *parent = 0);
    ~TargetWindow();

    void setDiap(const int diap) override;
    void setMarker(const double x, const double y) override;

    void run() override;
    void stop() override;

    /*!
     * \brief Возвращает результат работы окна пациента
     */
    QVariant result() override;

private:
    Ui::TargetWindow *ui;
};

#endif // TARGETWINDOW_H
