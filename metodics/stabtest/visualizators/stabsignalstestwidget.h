#ifndef STABSIGNALSTESTWIDGET_H
#define STABSIGNALSTESTWIDGET_H

#include <QWidget>
#include <QStandardItemModel>

class TestResultData;

namespace Ui {
class StabSignalsTestWidget;
}

class StabSignalsTestCalculator;
/*!
 * \brief Класс виджета визуализатора обработчика сигналов стабилографического теста StabSignalsTestWidget class
 */
class StabSignalsTestWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StabSignalsTestWidget(QWidget *parent = 0);
    ~StabSignalsTestWidget();

    /*!
     * \brief Метод расчета и отображения данных
     * \param testUid - uid теста
     */
    void calculate(StabSignalsTestCalculator *calculator, const QString &testUid);

private slots:
    void zoomIn();
    void zoomOut();
    void zeroing(bool isZeroing);

    void splitterMoved(int pos,int index);

private:

    enum FactorGroupId
    {
          fgiClassic
        , fgiVector
    };

    void showTable(StabSignalsTestCalculator *calculator, const QString &testUid);

    void addFactorsFromMultifactor(StabSignalsTestCalculator *calculator, const FactorGroupId fgi);
    int factorCount(StabSignalsTestCalculator *calculator, const FactorGroupId fgi) const;
    QString factorUid(StabSignalsTestCalculator *calculator, const FactorGroupId fgi, const int factorNum) const;
    double factorValue(StabSignalsTestCalculator *calculator, const FactorGroupId fgi,
                       const int probeNum, const int factorNum) const;

    void showSKG(StabSignalsTestCalculator *calculator, const QString &testUid);

    void saveSplitterPosition();
    void restoreSplitterPosition();

    Ui::StabSignalsTestWidget *ui;
    QStandardItemModel m_mdlTable;
    TestResultData* m_trd;
};

#endif // STABSIGNALSTESTWIDGET_H