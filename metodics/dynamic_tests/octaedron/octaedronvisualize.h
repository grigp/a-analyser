#ifndef OCTAEDRONVISUALIZE_H
#define OCTAEDRONVISUALIZE_H

#include <QWidget>

namespace Ui {
class OctaedronVisualize;
}

class OctaedronCalculator;

/*!
 * \brief Класс виджета показа результатов тренажера "Октаэдр" OctaedronVisualize class
 */
class OctaedronVisualize : public QWidget
{
    Q_OBJECT

public:
    explicit OctaedronVisualize(QWidget *parent = nullptr);
    ~OctaedronVisualize();

    void setTest(const QString &testUid);

private:
    Ui::OctaedronVisualize *ui;

    OctaedronCalculator* m_calculator {nullptr};
};

#endif // OCTAEDRONVISUALIZE_H