#ifndef TRIANGLEVISUALIZE_H
#define TRIANGLEVISUALIZE_H

#include <QWidget>
#include <QPrinter>

#include "baseutils.h"
#include "triangledefines.h"

namespace Ui {
class TriangleVisualize;
}

class TriangleCalculator;
class AreaSKG;
class Stabilogram;

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
    void showSKG(AreaSKG* area, BaseUtils::Section section);

    /*!
     * \brief Добавляет диаграмму треугольника в зону СКГ
     * \param area - указатель на зону СКГ
     * \param triangle - данные треугольника
     * \param color - цвет диаграммы
     * \return номер диаграммы треугольника в зоне построения
     */
    int addTriangleDiag(AreaSKG* area, TriangleDefines::Triangle triangle, QColor color);

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

    void saveSplitterPositionDiag();
    void restoreSplitterPositionDiag();

    TriangleCalculator* m_calculator {nullptr};
    Stabilogram* m_stab {nullptr};

    int m_curTriangleTraining {0};  ///< Отображаемый треугольник на этапе обучения в режиме одного треугольника
    int m_curTriangleAnalysis {0};  ///< Отображаемый треугольник на этапе анализа в режиме одного треугольника

    int m_triangleDiagIdxTraining {-1};   ///< Индекс диаграммы реального треугольника в зоне СКГ этапа обучения
    int m_triangleDiagIdxAnalysis {-1};   ///< Индекс диаграммы реального треугольника в зоне СКГ этапа анализа

};

#endif // TRIANGLEVISUALIZE_H
