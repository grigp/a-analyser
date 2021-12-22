#ifndef TRIANGLEVISUALIZE_H
#define TRIANGLEVISUALIZE_H

#include <QWidget>
#include <QPrinter>

#include "baseutils.h"

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

private:
    Ui::TriangleVisualize *ui;

    void getSignal(const QString &testUid);

    /*!
     * \brief Прорисовывает статокинезиграмму
     * \param area - зона построения
     * \param section - участок сигнала
     */
    void showSKG(AreaSKG* area, BaseUtils::Section section);

    void saveSplitterPositionDiag();
    void restoreSplitterPositionDiag();

    TriangleCalculator* m_calculator {nullptr};
    Stabilogram* m_stab {nullptr};

};

#endif // TRIANGLEVISUALIZE_H
