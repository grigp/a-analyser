#ifndef STEPDEVIATIONPATIENTWINDOW_H
#define STEPDEVIATIONPATIENTWINDOW_H

#include "stabdynamictestpatientwindow.h"
#include "basedefines.h"

#include <QObject>
#include <QWidget>
#include <QLabel>

class StepDeviationPatientWindow : public StabDynamicTestPatientWindow
{
    Q_OBJECT
public:
    explicit StepDeviationPatientWindow(BaseDefines::Directions direction, QWidget *parent = nullptr);
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
    QLabel* m_infoDir {nullptr};
    BaseDefines::Directions m_direction {BaseDefines::dirUp};
};

#endif // STEPDEVIATIONPATIENTWINDOW_H
