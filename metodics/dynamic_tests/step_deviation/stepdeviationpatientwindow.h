#ifndef STEPDEVIATIONPATIENTWINDOW_H
#define STEPDEVIATIONPATIENTWINDOW_H

#include "stabdynamictestpatientwindow.h"
#include "baseutils.h"

#include <QObject>
#include <QWidget>
#include <QLabel>

class StepDeviationPatientWindow : public StabDynamicTestPatientWindow
{
    Q_OBJECT
public:
    explicit StepDeviationPatientWindow(BaseUtils::Directions direction, QWidget *parent = nullptr);
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
    BaseUtils::Directions m_direction {BaseUtils::dirUp};
};

#endif // STEPDEVIATIONPATIENTWINDOW_H
