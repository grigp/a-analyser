#ifndef TRIANGLEEXECUTE_H
#define TRIANGLEEXECUTE_H

#include <QObject>

#include "stabdynamictestexecute.h"
#include "baseutils.h"
#include "triangledefines.h"

class SetMaxForceDialog;
class TrianglePatientWindow;

/*!
 * \brief Структура записи о треугольнике Target struct
 */
struct Target
{
    double x;
    double y;
    BaseUtils::TriangleCorner corner;
    Target(double _x, double _y, BaseUtils::TriangleCorner _corner)
        :x(_x), y(_y), corner(_corner){}
};

/*!
 * \brief Класс проведения теста "Треугольник" TriangleExecute class
 */
class TriangleExecute : public StabDynamicTestExecute
{
    Q_OBJECT
public:
    explicit TriangleExecute(QWidget *parent = nullptr);
    ~TriangleExecute() override;

    void setParams(const QJsonObject &params) override;

protected:
    /*!
     * \brief Виртуальная функция, создающая окно пациента. Ложна быть перекрыта в подклассах
     */
    StabDynamicTestPatientWindow* createPatientWindow() override;

    /*!
     * \brief Действия по завершению теста
     */
    void finishTest() override;

    /*!
     * \brief Перекрытая функция заполнения панели специфичных элеменов управления для методики
     * \param frSpecific - указатель на фрейм
     */
    void fillSpecific(QFrame *frSpecific) override;


protected slots:
    void start() override;

    /*!
     * \brief Управляет началом / остановом / прерыванием записи
     */
    void recording() override;

    void getData(DeviceProtocols::DeviceData *data) override;

    void on_communicationError(const QString &drvName, const QString &port, const int errorCode) override;

private slots:
    void setMaxForceDialogAccepted();

private:
    /*!
     * \brief Показывает текущий выбранный угол
     */
    void showCurrentCorner();

    /*!
     * \brief Выьор следующей вершины
     */
    void nextCorner();

    BaseUtils::DirectionMode m_directionMode {BaseUtils::dmClockwise};
    int m_trainingTime {60};
    int m_analysisTime {120};
    int m_forcePercent {75};
    int m_stageTime {2};
    int m_minDeviation {50};
    bool m_showMarkerAnal {false};

    TrianglePatientWindow* m_patientWin {nullptr};
    SetMaxForceDialog* m_mfd {nullptr};
    QList<Target> m_targets;

    TriangleDefines::Stage m_stage {TriangleDefines::stgWaiting};   ///< Этап теста
    BaseUtils::TriangleCorner m_curCorner {BaseUtils::tcNone};

    int m_stageCounter {0};   ///< Счетчик отсчетов на этапе
    int m_startAnalysis {0};  ///< Точка начала этапа анализа (в отсчетах)

};

#endif // TRIANGLEEXECUTE_H
