#ifndef STEPOFFSETTPATIENTWINDOW_H
#define STEPOFFSETTPATIENTWINDOW_H

#include <QWidget>
#include "stabdynamictestpatientwindow.h"

namespace Ui {
class StepOffsettPatientWindow;
}

class StepOffsettPatientWindow : public StabDynamicTestPatientWindow
{
    Q_OBJECT

public:
    explicit StepOffsettPatientWindow(QWidget *parent = nullptr);
    ~StepOffsettPatientWindow() override;

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
    Ui::StepOffsettPatientWindow *ui;
};

#endif // STEPOFFSETTPATIENTWINDOW_H
