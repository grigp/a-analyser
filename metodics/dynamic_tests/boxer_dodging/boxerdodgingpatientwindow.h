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

    /*!
     * \brief Виртуальная функция, помещающая комментарий поверх окна
     * \param comment - текст комментария
     */
    virtual void setFrontComment(const QString& comment, const QString& styleSheet = "");

protected:
    void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::BoxerDodgingPatientWindow *ui;
};

#endif // BOXERDODGINGPATIENTWINDOW_H
