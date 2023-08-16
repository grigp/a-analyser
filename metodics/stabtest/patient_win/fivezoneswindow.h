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
    explicit FiveZonesWindow(QWidget *parent = nullptr);
    ~FiveZonesWindow() override;

    void setDiap(const int diap) override;
    void setMarker(const double x, const double y) override;

    void run() override;
    void stop() override;

    /*!
     * \brief Функция, помещающая комментарий поверх окна
     * \param comment - текст комментария
     */
    void setFrontComment(const QString& comment) override;

    /*!
     * \brief Возвращает результат работы окна пациента
     */
    QVariant result() override;

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::FiveZonesWindow *ui;
};

#endif // FIVEZONESWINDOW_H
