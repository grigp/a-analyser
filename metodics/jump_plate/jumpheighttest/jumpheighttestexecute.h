#ifndef JUMPHEIGHTTESTEXECUTE_H
#define JUMPHEIGHTTESTEXECUTE_H

#include <QWidget>
#include <QJsonObject>

#include "jumpheighttestdefines.h"


namespace Ui {
class JumpHeightTestExecute;
}

/*!
 * \brief Виджет выполнения тестирования измерения высоты серии прыжков JumpHeightTestExecute class
 */
class JumpHeightTestExecute : public QWidget
{
    Q_OBJECT

public:
    explicit JumpHeightTestExecute(QWidget *parent = 0);
    ~JumpHeightTestExecute();

    void setParams(const QJsonObject &params);

private:
    Ui::JumpHeightTestExecute *ui;

    JumpHeightTestDefines::TestFinishKind m_testFinishKind {JumpHeightTestDefines::tfkQuantity};
    JumpHeightTestDefines::Strategy m_strategy {JumpHeightTestDefines::jhsMaxHeight};
    int m_jumpsCount {1};
    int m_testTime {30};
    int m_contactTimeBound {200};
};

#endif // JUMPHEIGHTTESTEXECUTE_H
