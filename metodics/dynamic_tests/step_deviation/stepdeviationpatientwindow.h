#ifndef STEPDEVIATIONPATIENTWINDOW_H
#define STEPDEVIATIONPATIENTWINDOW_H

#include "stabdynamictestpatientwindow.h"

#include <QObject>
#include <QWidget>
#include <QLabel>

class StepDeviationPatientWindow : public StabDynamicTestPatientWindow
{
    Q_OBJECT
public:
    explicit StepDeviationPatientWindow(QWidget *parent = nullptr);
    ~StepDeviationPatientWindow() override;


    /*!
     * \brief Действия при начале записи
     */
    void run() override;

    /*!
     * \brief Действия при завершении / прерывании записи
     */
    void stop() override;

private:
    void setInfoLabel();

    QLabel* m_info {nullptr};
};

#endif // STEPDEVIATIONPATIENTWINDOW_H
