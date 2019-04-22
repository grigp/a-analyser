#ifndef JUMPTESTEXECUTE_H
#define JUMPTESTEXECUTE_H

#include <QWidget>

namespace Ui {
class JumpTestExecute;
}

/*!
 * \brief Виджет выполнения тестирования измерения высоты прыжка JumpTestExecute class
 */
class JumpTestExecute : public QWidget
{
    Q_OBJECT

public:
    explicit JumpTestExecute(QWidget *parent = 0);
    ~JumpTestExecute();

    void setParams(const QJsonObject &params);

private:
    Ui::JumpTestExecute *ui;
};

#endif // JUMPTESTEXECUTE_H
