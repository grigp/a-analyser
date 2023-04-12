#ifndef STABSIGNALSTESTWIDGET_H
#define STABSIGNALSTESTWIDGET_H

#include <QWidget>
#include <QTreeView>
#include <QStandardItemModel>
#include <QPrinter>

#include "datadefines.h"

class TestResultData;
class SKGWidget;
class AreaGraph;

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

public slots:
    void setVisible(bool visible) override;

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

    static bool isRombergTest(DataDefines::TestInfo ti);

    QString getKoefRombResume(const double value, const int format) const;

    QString getOffsetResume(const double value, const char chan) const;

    QString getDeviationResume(const double value) const;

    static QList<int> getProbesKind(const QJsonObject params);

    void addFactorsFromMultifactor(StabSignalsTestCalculator *calculator, const FactorGroupId fgi);
    int factorCount(StabSignalsTestCalculator *calculator, const FactorGroupId fgi) const;
    QString factorUid(StabSignalsTestCalculator *calculator, const FactorGroupId fgi, const int factorNum) const;
    double factorValue(StabSignalsTestCalculator *calculator, const FactorGroupId fgi,
                       const int probeNum, const int factorNum) const;

    void showSKG(StabSignalsTestCalculator *calculator, const QString &testUid);

    /*!
     * \brief Установка диапазонов для всех СКГ
     */
    void setDiapazones();

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
     * \param visual - указатель на виджет визуализатора
     */
    static void printOnePortrait(QPrinter *printer, QPainter *painter, const QString &testUid, const QRect paper, StabSignalsTestWidget* visual);
    /*!
     * \brief Печать отчета одна проба, ландшафтная ориентация
     */
    static void printOneLandscape(QPrinter *printer, QPainter *painter, const QString &testUid, const QRect paper, StabSignalsTestWidget* visual);
    /*!
     * \brief Печать отчета две пробы, портретная ориентация
     */
    static void printTwoPortrait(QPrinter *printer, QPainter *painter, const QString &testUid, const QRect paper, StabSignalsTestWidget* visual);
    /*!
     * \brief Печать отчета две пробы, ландшафтная ориентация
     */
    static void printTwoLandscape(QPrinter *printer, QPainter *painter, const QString &testUid, const QRect paper, StabSignalsTestWidget* visual);
    /*!
     * \brief Печать отчета три пробы, портретная ориентация
     */
    static void printThreePortrait(QPrinter *printer, QPainter *painter, const QString &testUid, const QRect paper, StabSignalsTestWidget* visual);
    /*!
     * \brief Печать отчета три пробы, ландшафтная ориентация
     */
    static void printThreeLandscape(QPrinter *printer, QPainter *painter, const QString &testUid, const QRect paper, StabSignalsTestWidget* visual);
    /*!
     * \brief Печать отчета пять проб, портретная ориентация
     */
    static void printFivePortrait(QPrinter *printer, QPainter *painter, const QString &testUid, const QRect paper, StabSignalsTestWidget* visual);
    /*!
     * \brief Печать отчета пять проб, ландшафтная ориентация
     */
    static void printFiveLandscape(QPrinter *printer, QPainter *painter, const QString &testUid, const QRect paper, StabSignalsTestWidget* visual);

    /*!
     * \brief Печать графиков в режиме пяти проб
     */
    static void printGraphFive(QPainter *painter, const QString &testUid, const QRect paper, StabSignalsTestWidget* visual);

    /*!
     * \brief Возвращает список названий проб
     * \param testUid - uid теста
     * \param count - кол-во проб в выходном списке
     */
    static QStringList getProbesNameList(const QString& testUid, const int count);

    Ui::StabSignalsTestWidget *ui;
    QStandardItemModel m_mdlTable;
    TestResultData* m_trd {nullptr};
    QStandardItemModel* m_mdlRF {nullptr};
    QStandardItemModel* m_mdlNorms {nullptr};
    double m_absMax {0};

    QTreeView *m_tvRombergNorms {nullptr};
    QList<SKGWidget*> m_areasesSKG;
    AreaGraph *m_wgtGraph;
    QTabWidget *m_wgtTab;
};

#endif // STABSIGNALSTESTWIDGET_H
