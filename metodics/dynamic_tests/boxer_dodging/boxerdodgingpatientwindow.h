#ifndef BOXERDODGINGPATIENTWINDOW_H
#define BOXERDODGINGPATIENTWINDOW_H

#include "boxerdodgingdefines.h"

#include <QDialog>

namespace Ui {
class BoxerDodgingPatientWindow;
}

/*!
 * \brief Класс окна пациентов тренажера с уклонением для боксеров BoxerDodgingPatientWindow class
 */
class BoxerDodgingPatientWindow : public QDialog
{
    Q_OBJECT

public:
    explicit BoxerDodgingPatientWindow(QWidget *parent = nullptr);
    ~BoxerDodgingPatientWindow();

    /*!
     * \brief Устанавливает позицию боксера для этапа
     * \param stage - код этапа
     * \param visible - видимость
     */
    void setVisibleStage(const BoxerDodgingDefines::Stages stage, const bool visible);

private:
    Ui::BoxerDodgingPatientWindow *ui;
};

#endif // BOXERDODGINGPATIENTWINDOW_H
