#ifndef STABTESTEXECUTE_H
#define STABTESTEXECUTE_H

#include <QWidget>

namespace Ui {
class StabTestExecute;
}

/*!
 * \brief Виджет выполнения стабилометрического тестирования StabTestExecute class
 */
class StabTestExecute : public QWidget
{
    Q_OBJECT

public:
    explicit StabTestExecute(QWidget *parent = 0);
    ~StabTestExecute();

    void setParams(const QJsonObject &params);

private slots:
    void showDataBase();


private:
    Ui::StabTestExecute *ui;
};

#endif // STABTESTEXECUTE_H
