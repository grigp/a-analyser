#ifndef CROSSEXECUTE_H
#define CROSSEXECUTE_H

#include <QWidget>
#include <QJsonObject>

#include "datadefines.h"
#include "crossdefines.h"
#include "stabdynamictestexecute.h"
#include "deviceprotocols.h"

namespace Ui {
class CrossExecute;
}

/*!
 * \brief Виджет выполнения теста на устойчивость CrossExecute class
 */
class CrossExecute : public StabDynamicTestExecute
{
    Q_OBJECT

public:
    explicit CrossExecute(QWidget *parent = nullptr);
    ~CrossExecute() override;

    void setParams(const QJsonObject &params) override;

protected:

protected slots:
    void start() override;

    /*!
     * \brief Управляет началом / остановом / прерыванием записи
     */
    void recording() override;

    void getData(DeviceProtocols::DeviceData *data) override;

    void on_communicationError(const QString &drvName, const QString &port, const int errorCode) override;


private:
    Ui::CrossExecute *ui;

    int m_stageTime {10};
    int m_repeatCount {1};
    int m_centerSize {15};
    CrossDefines::ChangeStateMode m_changeStateMode{CrossDefines::csmReturn};
    CrossDefines::DirectionMode m_directionMode{CrossDefines::dmRandom};

};

#endif // CROSSEXECUTE_H
