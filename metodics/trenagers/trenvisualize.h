#ifndef TRENVISUALIZE_H
#define TRENVISUALIZE_H

#include <QWidget>

namespace Ui {
class TrenVisualize;
}

class TrenTestCalculator;

/*!
 * \brief Класс виджета показа результатов тренажеров TrenVisualize class
 */
class TrenVisualize : public QWidget
{
    Q_OBJECT

public:
    explicit TrenVisualize(QWidget *parent = nullptr);
    ~TrenVisualize() override;

    void setTest(const QString &testUid);


private:
    Ui::TrenVisualize *ui;

    TrenTestCalculator* m_calculator {nullptr};
};

#endif // TRENVISUALIZE_H
