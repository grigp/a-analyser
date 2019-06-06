#ifndef JUMPTESTVISUALIZE_H
#define JUMPTESTVISUALIZE_H

#include <QWidget>

namespace Ui {
class JumpTestVisualize;
}

/*!
 * \brief Виджет показа результатов измерения высоты прыжка JumpTestVisualize class
 */
class JumpTestVisualize : public QWidget
{
    Q_OBJECT

public:
    explicit JumpTestVisualize(QWidget *parent = 0);
    ~JumpTestVisualize();

    void setTest(const QString &testUid);

private:
    Ui::JumpTestVisualize *ui;
};

#endif // JUMPTESTVISUALIZE_H
