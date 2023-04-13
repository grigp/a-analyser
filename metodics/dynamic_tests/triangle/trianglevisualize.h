#ifndef TRIANGLEVISUALIZE_H
#define TRIANGLEVISUALIZE_H

#include <QWidget>
#include <QPrinter>
#include <QStandardItemModel>
#include <QTabWidget>

#include "basedefines.h"
#include "triangledefines.h"

namespace Ui {
class TriangleVisualize;
}

class TriangleCalculator;
class SKGWidget;
class Stabilogram;
class DualStateDiagram;

/*!
 * \brief Класс виджета показа результатов теста Треугольник TriangleVisualize class
 */
class TriangleVisualize : public QWidget
{
    Q_OBJECT

public:
    explicit TriangleVisualize(QWidget *parent = nullptr);
    ~TriangleVisualize();

    void setTest(const QString &testUid);

    /*!
     * \brief Печать отчета о результатах теста
     * \param printer - принтер
     * \param testUid - uid теста
     */
    static void print(QPrinter *printer, const QString &testUid);

private slots:
    void curPageChanged(int pageIdx);
    void splitterDiagMoved(int pos,int index);

    void btnAllTrainingClicked(bool pressed);
    void btnAllAnalysisClicked(bool pressed);

    void btnPrevTrainingClicked();
    void btnNextTrainingClicked();
    void setBtnPNTrainingEnabled();
    void btnPrevAnalysisClicked();
    void btnNextAnalysisClicked();
    void setBtnPNAnalysisEnabled();

    void cbShowTraectoryTrainingClicked(bool isChecked);
    void cbShowTraectoryAnalysisClicked(bool isChecked);

    void tbPlusTrainingClicked();
    void tbMinusTrainingClicked();
    void tbPlusAnalysisClicked();
    void tbMinusAnalysisClicked();

private:
    Ui::TriangleVisualize *ui;

    void getSignal(const QString &testUid);

    /*!
     * \brief Прорисовывает статокинезиграмму
     * \param area - зона построения
     * \param section - участок сигнала
     */
    void showSKG(SKGWidget* area, BaseDefines::Section section);

    /*!
     * \brief Добавляет диаграмму треугольника в зону СКГ
     * \param area - указатель на зону СКГ
     * \param triangle - данные треугольника
     * \param color - цвет диаграммы
     * \return номер диаграммы треугольника в зоне построения
     */
    int addTriangleDiag(SKGWidget* area, TriangleDefines::Triangle triangle, QColor color);

    /*!
     * \brief Выводит польную таблицу показателей
     */
    void showAllFactors();

    /*!
     * \brief Показывает основные диаграммы
     */
    void showMainDiagrams();

    /*!
     * \brief Выводит показатели основных диаграмм
     */
    void showMainResultFactors();

    /*!
     * \brief Выводит показатели диаграмм треугольников
     */
    void showDiagsResultFactors();

    /*!
     * \brief Прорисовывает закладку "Заключение"
     */
    void showConslution();

    /*!
     * \brief Вывод таблицы эффективности
     */
    void showFactorsEffectiveness();

    /*!
     * \brief Показывает заключение по стратегии
     */
    void showConslutionStrategy();

    void saveSplitterPositionDiag();
    void restoreSplitterPositionDiag();

    TriangleCalculator* m_calculator {nullptr};
    Stabilogram* m_stab {nullptr};

    int m_curTriangleTraining {0};  ///< Отображаемый треугольник на этапе обучения в режиме одного треугольника
    int m_curTriangleAnalysis {0};  ///< Отображаемый треугольник на этапе анализа в режиме одного треугольника

    int m_triangleDiagIdxTraining {-1};   ///< Индекс диаграммы реального треугольника в зоне СКГ этапа обучения
    int m_triangleDiagIdxAnalysis {-1};   ///< Индекс диаграммы реального треугольника в зоне СКГ этапа анализа

    SKGWidget *m_wgtSKGTraining {nullptr};
    SKGWidget *m_wgtSKGAnalysis {nullptr};

    QStandardItemModel *m_mdlDiagTable {nullptr};
    QStandardItemModel *m_mdlEffectiveness {nullptr};
    QStandardItemModel *m_mdlMainTable {nullptr};
    QTabWidget *m_twPages {nullptr};

    QString m_sLatentMoving {""};
    QString m_sAccRepeat {""};
    QString m_sAccForm {""};
    QString m_sAccMidX {""};
    QString m_sAccMidY {""};
    QString m_sMidSquareErrTst {""};
    QString m_sMidTimeErrAnl {""};
    QString m_sMidSquareErrAnl {""};
    QString m_sMidPosErrAnl {""};
    QString m_sMidAmplErrAnl {""};

    QString m_sCorrectionPredominaceTrain {""};
    QString m_sCorrectionResumeTrain {""};
    QColor m_sCorrectionResumeColorTrain {Qt::black};
    DualStateDiagram * m_wgtCorrectionDiagTrain {nullptr};
    QString m_sCorrectionPredominaceAnal {""};
    QString m_sCorrectionResumeAnal {""};
    QColor m_sCorrectionResumeColorAnal {Qt::black};
    DualStateDiagram * m_wgtCorrectionDiagAnal {nullptr};
};

#endif // TRIANGLEVISUALIZE_H
