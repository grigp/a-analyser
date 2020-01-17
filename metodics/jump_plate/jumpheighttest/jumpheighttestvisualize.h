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

private:
    Ui::JumpHeightTestVisualize *ui;

    void getStrategyParams(const QString &testUid);

    JumpHeightTestCalculator* m_calculator {nullptr};
    JumpHeightTestDefines::Strategy m_strategy {JumpHeightTestDefines::jhsMaxHeight};
    double m_contactTimeBound {0};
};

#endif // JUMPHEIGHTTESTVISUALIZE_H
