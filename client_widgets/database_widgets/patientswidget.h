#ifndef PATIENTSWIDGET_H
#define PATIENTSWIDGET_H

#include <QWidget>

class PatientsModel;

namespace Ui {
class PatientsWidget;
}

class PatientsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PatientsWidget(QWidget *parent = 0);
    ~PatientsWidget();

    void onDbConnect();

private:
    Ui::PatientsWidget *ui;

    PatientsModel *m_mdlPatients;
};

#endif // PATIENTSWIDGET_H
