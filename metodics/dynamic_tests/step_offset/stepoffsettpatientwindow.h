#ifndef STEPOFFSETTPATIENTWINDOW_H
#define STEPOFFSETTPATIENTWINDOW_H

#include <QWidget>
#include "stabdynamictestpatientwindow.h"

namespace Ui {
class StepOffsettPatientWindow;
}

/*!
 * \brief Класс окна пациента теста "Ступени" (со ступенчатым воздействием) The StepOffsettPatientWindow class
 */
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

    /*!
     * \brief Перекрытая функция установки диапазона изменения координат
     * \param diap - диапазон
     */
    void setDiap(const int diap) override;

    /*!
     * \brief Установка специфичного диапаона
     * \param diap - диапазон
     */
    void setDiapSpecific(const int diap);

private:
    Ui::StepOffsettPatientWindow *ui;

    int m_diap {128};
};

#endif // STEPOFFSETTPATIENTWINDOW_H
