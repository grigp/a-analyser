#ifndef STABILOGRAMWIDGET_H
#define STABILOGRAMWIDGET_H

#include <QWidget>

#include "signalwidget.h"

namespace Ui {
class StabilogramWidget;
}

/*!
 * \brief Класс виджета - стабилограммы StabilogramWidget class
 */
class StabilogramWidget : public SignalWidget
{
    Q_OBJECT

public:
    explicit StabilogramWidget(const QString channelId, QWidget *parent = 0);
    ~StabilogramWidget();

    /*!
     * \brief Получение данных от устройства
     * \param data - данные
     */
    void getData(DeviceProtocols::DeviceData *data) override;

    void setFrequency(const int frequency) override;

private:
    Ui::StabilogramWidget *ui;
};

#endif // STABILOGRAMWIDGET_H
