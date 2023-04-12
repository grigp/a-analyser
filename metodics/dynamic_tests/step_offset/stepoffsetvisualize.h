#ifndef STEPOFFSETVISUALIZE_H
#define STEPOFFSETVISUALIZE_H

#include <QWidget>
#include <QPrinter>
#include <QStandardItemModel>
#include <QTabWidget>

namespace Ui {
class StepOffsetVisualize;
}

class StepOffsetCalculator;
class TransientsPainter;
class Transients;
class DualStateDiagram;

/*!
 * \brief Класс виджета показа результатов теста "Ступени" StepOffsetVisualize class
 */
class StepOffsetVisualize : public QWidget
{
    Q_OBJECT

public:
    explicit StepOffsetVisualize(QWidget *parent = nullptr);
    ~StepOffsetVisualize();

    void setTest(const QString &testUid);

    /*!
     * \brief Печать отчета о результатах теста
     * \param printer - принтер
     * \param testUid - uid теста
     */
    static void print(QPrinter *printer, const QString &testUid);

private slots:
    void curPageChanged(int pageIdx);
    void splitterMoved(int pos,int index);

private:
    Ui::StepOffsetVisualize *ui;

    void saveSplitterPosition();
    void restoreSplitterPosition();

    /*!
     * \brief Показывает переходные процессы
     */
    void showTransients();

    /*!
     * \brief Показывает таблицу показателей
     */
    void showTable();

    /*!
     * \brief Показывает параметры теста
     */
    void showParams();

    /*!
     * \brief Показывает заключение
     */
    void showConslution();

    /*!
     * \brief Показывает заключение по стратегии
     */
    void showConslutionStrategy();

    static void printTransistents(TransientsPainter* tp, StepOffsetVisualize* visual);

    StepOffsetCalculator* m_calculator {nullptr};

    Transients *m_wgtProcess {nullptr};
    QStandardItemModel *m_mdlFactors {nullptr};
    QString m_sForce {""};
    QString m_sDirection {""};
    QString m_sDeviation {""};
    QString m_sRepeatCount {""};

    QString m_sReactionTime {""};
    QString m_sStatism {""};
    QString m_sLatent {""};
    QString m_sSpurtSpeed {""};
    QString m_sSpurtAmpl {""};
    QString m_sStabilityDeviation {""};
    QString m_sRetentionDeviation {""};
    QString m_sPrecisionKogn {""};
    QString m_sPrecisionMotor {""};
    QString m_sProcessKind {""};
    QString m_sCorrectionPredominace {""};
    QString m_sCorrectionResume {""};
    QColor m_sCorrectionResumeColorTrain;
    DualStateDiagram *m_wgtCorrectionDiagTrain {nullptr};

    QTabWidget *m_wgtTab {nullptr};
};

#endif // STEPOFFSETVISUALIZE_H
