#ifndef BOXERDODGINGVISUALIZE_H
#define BOXERDODGINGVISUALIZE_H

#include <QWidget>

namespace Ui {
class BoxerDodgingVisualize;
}

class BoxerDodgingCalculator;

/*!
 * \brief Класс фрейма визуализации результатов тренажера с уклонением для боксеров BoxerDodgingVisualize class
 */
class BoxerDodgingVisualize : public QWidget
{
    Q_OBJECT

public:
    explicit BoxerDodgingVisualize(QWidget *parent = nullptr);
    ~BoxerDodgingVisualize() override;

    void setTest(const QString &testUid);

private:
    Ui::BoxerDodgingVisualize *ui;

    BoxerDodgingCalculator* m_calculator {nullptr};
};

#endif // BOXERDODGINGVISUALIZE_H
