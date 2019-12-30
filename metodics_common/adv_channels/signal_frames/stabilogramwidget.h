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
    explicit StabilogramWidget(Driver *driver, const QString channelId, QWidget *parent = 0);
    ~StabilogramWidget();

    /*!
     * \brief Получение данных от устройства
     * \param data - данные
     */
    void getData(DeviceProtocols::DeviceData *data) override;

    void setFrequency(const int frequency) override;

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void on_changeScaleStab(int scaleIdx);
    void on_changeScaleZ(int scaleIdx);

    void on_zeroingStab();
    void on_calibrate();

private:
    Ui::StabilogramWidget *ui;

    QVector<double> m_recStab;
    QVector<double> m_recZ;
    bool m_isShowMassa {false};
};

#endif // STABILOGRAMWIDGET_H
