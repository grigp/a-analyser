#ifndef EVOLVENTAPATIENTWINDOW_H
#define EVOLVENTAPATIENTWINDOW_H

#include <QDialog>

#include "stabdynamictestpatientwindow.h"

namespace Ui {
class EvolventaPatientWindow;
}

class EvolventaPatientWindow : public StabDynamicTestPatientWindow
{
    Q_OBJECT

public:
    explicit EvolventaPatientWindow(QWidget *parent = nullptr);
    ~EvolventaPatientWindow() override;

    /*!
     * \brief Установка маркера
     * \param x, y - координаты маркера
     */
    void setMarker(const double x, const double y) override;

    /*!
     * \brief Действия при начале записи
     */
    void run() override;

    /*!
     * \brief Действия при завершении / прерывании записи
     */
    void stop() override;

private:
    Ui::EvolventaPatientWindow *ui;
};

#endif // EVOLVENTAPATIENTWINDOW_H
