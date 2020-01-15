#ifndef JUMPHEIGHTTESTVISUALIZE_H
#define JUMPHEIGHTTESTVISUALIZE_H

#include <QWidget>

namespace Ui {
class JumpHeightTestVisualize;
}

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
};

#endif // JUMPHEIGHTTESTVISUALIZE_H
