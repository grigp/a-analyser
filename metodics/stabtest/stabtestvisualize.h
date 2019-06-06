#ifndef STABTESTVISUALIZE_H
#define STABTESTVISUALIZE_H

#include <QWidget>

namespace Ui {
class StabTestVisualize;
}

/*!
 * \brief Виджет показа результатов стабилометрического тестирования StabTestVisualize class
 */
class StabTestVisualize : public QWidget
{
    Q_OBJECT

public:
    explicit StabTestVisualize(QWidget *parent = 0);
    ~StabTestVisualize();

    void setTest(const QString &testUid);

private:
    Ui::StabTestVisualize *ui;
};

#endif // STABTESTVISUALIZE_H
