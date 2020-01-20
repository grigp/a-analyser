#ifndef DROPTESTVISUALIZE_H
#define DROPTESTVISUALIZE_H

#include <QWidget>

namespace Ui {
class DropTestVisualize;
}

/*!
 * \brief Виджет показа результатов дроп теста DropTestVisualize class
 */
class DropTestVisualize : public QWidget
{
    Q_OBJECT

public:
    explicit DropTestVisualize(QWidget *parent = 0);
    ~DropTestVisualize();

    void setTest(const QString &testUid);

private:
    Ui::DropTestVisualize *ui;
};

#endif // DROPTESTVISUALIZE_H
