#ifndef FIVEZONESWINDOW_H
#define FIVEZONESWINDOW_H

#include <QDialog>

#include "patientwindow.h"

namespace Ui {
class FiveZonesWindow;
}

/*!
 * \brief Форма пациента "С выделенной зоной" FiveZonesWindow class
 */
class FiveZonesWindow : public PatientWindow
{
    Q_OBJECT

public:
    explicit FiveZonesWindow(QWidget *parent = 0);
    ~FiveZonesWindow();

    void setDiap(const int diap) override;
    void setMarker(const double x, const double y) override;

    void run() override;
    void stop() override;

    /*!
     * \brief Возвращает результат работы окна пациента
     */
    QVariant result() override;


private:
    Ui::FiveZonesWindow *ui;
};

#endif // FIVEZONESWINDOW_H
