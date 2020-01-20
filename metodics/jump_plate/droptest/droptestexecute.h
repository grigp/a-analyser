#ifndef DROPTESTEXECUTE_H
#define DROPTESTEXECUTE_H

#include <QWidget>
#include <QJsonObject>

#include "jumpplatedefines.h"

namespace Ui {
class DropTestExecute;
}

/*!
 * \brief Виджет выполнения дроп теста DropTestExecute class
 */
class DropTestExecute : public QWidget
{
    Q_OBJECT

public:
    explicit DropTestExecute(QWidget *parent = 0);
    ~DropTestExecute();

    void setParams(const QJsonObject &params);

private:
    Ui::DropTestExecute *ui;

    JumpPlateDefines::TestFinishKind m_testFinishKind {JumpPlateDefines::tfkQuantity};
    int m_jumpsMax {1};
    int m_testTime {30};
};

#endif // DROPTESTEXECUTE_H
