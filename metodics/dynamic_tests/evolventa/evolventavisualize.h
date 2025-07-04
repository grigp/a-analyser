#ifndef EVOLVENTAVISUALIZE_H
#define EVOLVENTAVISUALIZE_H

#include <QWidget>
#include <QPrinter>
#include <QStandardItemModel>
#include <QLabel>

#include "evolventadefines.h"

namespace Ui {
class EvolventaVisualize;
}

class EvolventaCalculator;
class AreaGraph;
class DualStateDiagram;
class DecartCoordinatesSignal;

/*!
 * \brief Класс виджета показа результатов теста Эвольвента EvolventaVisualize class
 */
class EvolventaVisualize : public QWidget
{
    Q_OBJECT

public:
    explicit EvolventaVisualize(QWidget *parent = nullptr);
    ~EvolventaVisualize() override;

    void setTest(const QString &testUid);

    /*!
     * \brief Печать отчета о результатах теста
     * \param printer - принтер
     * \param testUid - uid теста
     */
    static void print(QPrinter *printer, const QString &testUid);

    static void printGraph(QPainter *painter,
                           EvolventaVisualize* visual, double ratio,
                           QRect rect, DecartCoordinatesSignal *signal);

    /*!
     * \brief Прорисовывает предварительные результаты с помощью заданного рисователя
     * \param painter - рисователь
     * \param rect - зона прорисовки
     * \param testUid - uid теста
     */
    static void paintPreview(QPainter* painter, QRect& rect, const QString& testUid, EvolventaCalculator* calculator);

private slots:
    /*!
     * \brief Сохраняет таблицу показателей коррекций
     */
    void saveFactorsCorrections();

    /*!
     * \brief Сохраняет эвольвенту
     */
    void saveEvolventa();

private:
    Ui::EvolventaVisualize *ui;

    /*!
     * \brief Показывает эвольвенту
     */
    void showEvolventa();

    /*!
     * \brief Показывает таблицу основных показателей
     */
    void showMainFactorsTable();

    /*!
     * \brief Показывает таблицу показателей коррекций
     */
    void showKorrectionFactorsTable();

    void setTableSpecialStyle();

    /*!
     * \brief Показывает показатели без таблиц
     */
    void showWithoutTableFactors();

    EvolventaCalculator* m_calculator {nullptr};

    AreaGraph *m_wgtGraphX {nullptr};
    AreaGraph *m_wgtGraphY {nullptr};
    QStandardItemModel *m_mdlMain {nullptr};
    QStandardItemModel *m_mdlKorrect {nullptr};
    DualStateDiagram *m_wgtOutrunningDiag {nullptr};
    QLabel *m_lblOutrunningValue {nullptr};
    QLabel *m_lblOutrunningResume {nullptr};
    DualStateDiagram *m_wgtCorrectionDominanceDiag {nullptr};
    QLabel *m_lblCorrectionDominanceValue {nullptr};
    QLabel *m_lblCorrectionDominanceResume {nullptr};

    EvolventaDefines::StageValueName m_name;
};

#endif // EVOLVENTAVISUALIZE_H
