#ifndef TRENTAKEPUTEXECUTE_H
#define TRENTAKEPUTEXECUTE_H

#include <QWidget>
#include <QJsonObject>

namespace Ui {
class TrenTakePutExecute;
}

/*!
 * \brief Класс виджета проведения сеанса тренинга TrenagerExecute class
 */
class TrenTakePutExecute : public QWidget
{
    Q_OBJECT

public:
    explicit TrenTakePutExecute(QWidget *parent = 0);
    ~TrenTakePutExecute();

    void setParams(const QJsonObject &params);

private:
    Ui::TrenTakePutExecute *ui;
};

#endif // TRENTAKEPUTEXECUTE_H
