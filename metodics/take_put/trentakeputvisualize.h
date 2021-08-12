#ifndef TRENTAKEPUTVISUALIZE_H
#define TRENTAKEPUTVISUALIZE_H

#include <QWidget>

namespace Ui {
class TrenTakePutVisualize;
}


class TrenTakePutTestCalculator;

/*!
 * \brief Виджет показа результатов тренажеров захвата - укладки TrenTakePutVisualize class
 */
class TrenTakePutVisualize : public QWidget
{
    Q_OBJECT

public:
    explicit TrenTakePutVisualize(QWidget *parent = 0);
    ~TrenTakePutVisualize();

    void setTest(const QString &testUid);

private:
    Ui::TrenTakePutVisualize *ui;

    TrenTakePutTestCalculator* m_calculator {nullptr};
};

#endif // TRENTAKEPUTVISUALIZE_H
