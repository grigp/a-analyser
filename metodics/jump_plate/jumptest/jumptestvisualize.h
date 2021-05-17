#ifndef JUMPTESTVISUALIZE_H
#define JUMPTESTVISUALIZE_H

#include <QWidget>

namespace Ui {
class JumpTestVisualize;
}

class JumpTestCalculator;

/*!
 * \brief Виджет показа результатов измерения высоты прыжка JumpTestVisualize class
 */
class JumpTestVisualize : public QWidget
{
    Q_OBJECT

public:
    explicit JumpTestVisualize(QWidget *parent = nullptr);
    ~JumpTestVisualize();

    void setTest(const QString &testUid);

private:
    Ui::JumpTestVisualize *ui;

    JumpTestCalculator* m_calculator {nullptr};
};

#endif // JUMPTESTVISUALIZE_H
