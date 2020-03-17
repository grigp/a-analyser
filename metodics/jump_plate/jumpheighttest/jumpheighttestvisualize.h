#ifndef JUMPHEIGHTTESTVISUALIZE_H
#define JUMPHEIGHTTESTVISUALIZE_H

#include <QWidget>

#include "jumpheighttestdefines.h"

namespace Ui {
class JumpHeightTestVisualize;
}

class JumpHeightTestCalculator;

/*!
 * \brief Виджет показа результатов измерения высоты серии прыжков JumpHeightTestVisualize class
 */
class JumpHeightTestVisualize : public QWidget
{
    Q_OBJECT

public:
    explicit JumpHeightTestVisualize(QWidget *parent = 0);
    ~JumpHeightTestVisualize();

    void setTest(const QString &testUid);

private slots:
    void on_selectGraph();
    void on_selectBar();
    void on_select3D(bool checked);

    void on_selectDiagItemHeight(const int idx);
    void on_selectDiagItemTime(const int idx);

    void on_selectPage(const int pageIdx);

private:
    Ui::JumpHeightTestVisualize *ui;

    void getStrategyParams(const QString &testUid);

    void setDiagrams();
    void setTable();

    void restoreGraphParams();

    JumpHeightTestCalculator* m_calculator {nullptr};
    JumpHeightTestDefines::Strategy m_strategy {JumpHeightTestDefines::jhsMaxHeight};
    double m_contactTimeBound {0};
};

#endif // JUMPHEIGHTTESTVISUALIZE_H
