#ifndef STABTESTVISUALIZE_H
#define STABTESTVISUALIZE_H

#include <QWidget>
#include <QTextEdit>

namespace Ui {
class StabTestVisualize;
}

class TestResultData;


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

    TestResultData* m_trd;

};

#endif // STABTESTVISUALIZE_H
