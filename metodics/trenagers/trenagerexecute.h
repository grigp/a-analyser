#ifndef TRENAGEREXECUTE_H
#define TRENAGEREXECUTE_H

#include <QWidget>
#include <QJsonObject>

namespace Ui {
class TrenagerExecute;
}

/*!
 * \brief Класс виджета проведения сеанса тренинга TrenagerExecute class
 */
class TrenagerExecute : public QWidget
{
    Q_OBJECT

public:
    explicit TrenagerExecute(QWidget *parent = 0);
    ~TrenagerExecute();

    void setParams(const QJsonObject &params);

private:
    Ui::TrenagerExecute *ui;
};

#endif // TRENAGEREXECUTE_H
