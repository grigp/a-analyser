#ifndef COLOREDCIRCLEWINDOW_H
#define COLOREDCIRCLEWINDOW_H

#include <QDialog>
#include "patientwindow.h"

namespace Ui {
class ColoredCircleWindow;
}

/*!
 * \brief Форма пациента "Цветные круги" ColoredCircleWindow class
 */
class ColoredCircleWindow : public PatientWindow
{
    Q_OBJECT

public:
    explicit ColoredCircleWindow(QWidget *parent = nullptr);
    ~ColoredCircleWindow() override;

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
    void timerEvent(QTimerEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::ColoredCircleWindow *ui;

    void nextColor();

    int m_tm;
    int m_targetColorCount {0};  ///< Кол-во кругов требуемого цвета
};

#endif // COLOREDCIRCLEWINDOW_H
