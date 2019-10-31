#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QWidget>

namespace Ui {
class Histogram;
}

/*!
 * \brief Класс виджета - гистограммы Histogram class
 */
class Histogram : public QWidget
{
    Q_OBJECT

public:
    explicit Histogram(QWidget *parent = 0);
    ~Histogram();

private:
    Ui::Histogram *ui;
};

#endif // HISTOGRAM_H
