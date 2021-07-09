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
    explicit EvolventaPatientWindow(const int diap, QWidget *parent = nullptr);
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

    /*!
     * \brief Перекрытая функция установки диапазона изменения координат
     * \param diap - диапазон
     */
    void setDiap(const int diap) override;

private:
    Ui::EvolventaPatientWindow *ui;

    int m_diap {128};
};

#endif // EVOLVENTAPATIENTWINDOW_H
