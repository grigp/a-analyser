#ifndef LINESWINDOW_H
#define LINESWINDOW_H

#include <QDialog>
#include "patientwindow.h"

namespace Ui {
class LinesWindow;
}

/*!
 * \brief Форма пациента с движущимися полосами The LinesWindow class
 */
class LinesWindow : public PatientWindow
{
    Q_OBJECT

public:
    explicit LinesWindow(QWidget *parent = nullptr);
    ~LinesWindow() override;

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

private:
    Ui::LinesWindow *ui;
};

#endif // LINESWINDOW_H
