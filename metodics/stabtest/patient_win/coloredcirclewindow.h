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
    explicit ColoredCircleWindow(QWidget *parent = 0);
    ~ColoredCircleWindow();

    void setStabData(const double x, const double y) override;

    void run() override;
    void stop() override;

protected:
    void timerEvent(QTimerEvent *event) override;

private:
    Ui::ColoredCircleWindow *ui;

    void nextColor();

    int m_tm;
};

#endif // COLOREDCIRCLEWINDOW_H
