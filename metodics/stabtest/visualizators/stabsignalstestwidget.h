#ifndef STABSIGNALSTESTWIDGET_H
#define STABSIGNALSTESTWIDGET_H

#include <QWidget>
#include <QTreeView>
#include <QStandardItemModel>
#include <QPrinter>

#include "datadefines.h"

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
    explicit StabSignalsTestWidget(QWidget *parent = nullptr);
    ~StabSignalsTestWidget() override;

    /*!
     * \brief Метод расчета и отображения данных
     * \param testUid - uid теста
     */
    void calculate(StabSignalsTestCalculator *calculator, const QString &testUid);

    /*!
     * \brief Печать отчета о результатах теста
     * \param printer - принтер
     * \param testUid - uid теста
     */
    static void print(QPrinter *printer, const QString &testUid);

    /*!
     * \brief Роли для модели таблицы нормативов для теста Ромберга
     */
    enum RombergNormsTableRoles
    {
          NormRole = Qt::UserRole + 1   ///< Норма. int. В колонках значений
        , ValueRole                     ///< Значение показателя. double. В колонках значений
        , FormatRole                    ///< Формат значения, кол-во знаков после запятой. int. В колонках значений
    };

protected:
    void resizeEvent(QResizeEvent *event) override;


private slots:
    void zoomIn();
    void zoomOut();
    void zeroing(bool isZeroing);

    void splitterMoved(int pos,int index);

    void curPageChanged(int pageIdx);
    void scaleChange(int scaleIdx);
    void fullSignalChanged(bool isFullSignal);
    void zeroingChanged(bool isZeroing);
    void signalScroll(int pos);
    void hScaleZoomIn();
    void hScaleZoomOut();

private:

    enum FactorGroupId
    {
          fgiClassic
        , fgiVector
    };

    void showTable(StabSignalsTestCalculator *calculator, const QString &testUid);

    void showRationalTable(StabSignalsTestCalculator *calculator, const QString &testUid);

    void showRombergNorms(StabSignalsTestCalculator *calculator, const QString &testUid);

    DataDefines::NormValue getRombergNorm(const int probeNum, const QString &factorUid, const double value) const;

    QStandardItem* createItemRationalFactors(StabSignalsTestCalculator *calculator,
                                             const int numProbe,
                                             const QString &factorUid, const int action, const char chan);

    void appendColumnReadOnly(QStandardItemModel *mdl, QList<QStandardItem*> list);

    bool isRombergTest(DataDefines::TestInfo ti);

    QString getKoefRombResume(const double value, const int format) const;

    QString getOffsetResume(const double value, const char chan) const;

    QString getDeviationResume(const double value) const;

    QList<int> getProbesKind(const QJsonObject params);

    void addFactorsFromMultifactor(StabSignalsTestCalculator *calculator, const FactorGroupId fgi);
    int factorCount(StabSignalsTestCalculator *calculator, const FactorGroupId fgi) const;
    QString factorUid(StabSignalsTestCalculator *calculator, const FactorGroupId fgi, const int factorNum) const;
    double factorValue(StabSignalsTestCalculator *calculator, const FactorGroupId fgi,
                       const int probeNum, const int factorNum) const;

    void showSKG(StabSignalsTestCalculator *calculator, const QString &testUid);

    void saveSplitterPosition();
    void restoreSplitterPosition();

    void resizeColumnsTable(QStandardItemModel *mdl, QTreeView* tv, const bool toContens);

    /*!
     * \brief Возвращает прямоугольник для футера страницы
     */
    static QRect rectFooter(const QRect &paper);

    /*!
     * \brief Печать отчета одна проба, портретная ориентация
     * \param printer - принтер
     * \param painter - указатель на рисователь
     * \param testUid - uid теста
     * \param paper - страница
     */
    static void printOnePortrait(QPrinter *printer, QPainter *painter, const QString &testUid, const QRect paper);
    /*!
     * \brief Печать отчета одна проба, ландшафтная ориентация
     */
    static void printOneLandscape(QPrinter *printer, QPainter *painter, const QString &testUid, const QRect paper);

    Ui::StabSignalsTestWidget *ui;
    QStandardItemModel m_mdlTable;
    TestResultData* m_trd {nullptr};
    QStandardItemModel* m_mdlRF {nullptr};
    QStandardItemModel* m_mdlNorms {nullptr};
};

#endif // STABSIGNALSTESTWIDGET_H
