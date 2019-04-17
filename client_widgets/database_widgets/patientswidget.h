#ifndef PATIENTSWIDGET_H
#define PATIENTSWIDGET_H

#include <QWidget>

namespace Ui {
class PatientsWidget;
}

class PatientsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PatientsWidget(QWidget *parent = 0);
    ~PatientsWidget();

private:
    Ui::PatientsWidget *ui;
};

#endif // PATIENTSWIDGET_H
