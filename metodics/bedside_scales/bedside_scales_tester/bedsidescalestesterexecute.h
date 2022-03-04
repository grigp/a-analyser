#ifndef BEDSIDESCALESTESTEREXECUTE_H
#define BEDSIDESCALESTESTEREXECUTE_H

#include <QWidget>
#include <QJsonObject>

namespace Ui {
class BedsideScalesTesterExecute;
}

/*!
 * \brief Виджет выполнения методики тестирования прикроватных весов The BedsideScalesTesterExecute class
 */
class BedsideScalesTesterExecute : public QWidget
{
    Q_OBJECT

public:
    explicit BedsideScalesTesterExecute(QWidget *parent = nullptr);
    ~BedsideScalesTesterExecute();

    void setParams(const QJsonObject &probeParams);

private:
    Ui::BedsideScalesTesterExecute *ui;
};

#endif // BEDSIDESCALESTESTEREXECUTE_H
