#ifndef CROSSPATIENTWINDOW_H
#define CROSSPATIENTWINDOW_H

#include <QDialog>

#include "stabdynamictestpatientwindow.h"

namespace Ui {
class CrossPatientWindow;
}

class CrossPatientWindow : public StabDynamicTestPatientWindow
{
    Q_OBJECT

public:
    explicit CrossPatientWindow(QWidget *parent = nullptr);
    ~CrossPatientWindow() override;

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
    Ui::CrossPatientWindow *ui;
};

#endif // CROSSPATIENTWINDOW_H
