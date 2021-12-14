#ifndef TRIANGLEPATIENTWINDOW_H
#define TRIANGLEPATIENTWINDOW_H

#include <QObject>

#include "stabdynamictestpatientwindow.h"

/*!
 * \brief Класс окна пациентов теста "Треугольник" TrianglePatientWindow class
 */
class TrianglePatientWindow : public StabDynamicTestPatientWindow
{
    Q_OBJECT
public:
    explicit TrianglePatientWindow(QWidget *parent = nullptr);

    /*!
     * \brief Действия при начале записи
     */
    void run() override;

    /*!
     * \brief Действия при завершении / прерывании записи
     */
    void stop() override;

    /*!
     * \brief Установка маркера
     * \param x, y - координаты маркера
     */
    void setMarker(const double x, const double y) override;

};

#endif // TRIANGLEPATIENTWINDOW_H
