#ifndef TARGETWINDOW_H
#define TARGETWINDOW_H

#include <QDialog>

#include "patientwindow.h"

namespace Ui {
class TargetWindow;
}


/*!
 * \brief Форма пациента "Мишень" TargetWindow class
 */
class TargetWindow : public PatientWindow
{
    Q_OBJECT

public:
    explicit TargetWindow(QWidget *parent = nullptr);
    ~TargetWindow() override;

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
    Ui::TargetWindow *ui;
};

#endif // TARGETWINDOW_H
