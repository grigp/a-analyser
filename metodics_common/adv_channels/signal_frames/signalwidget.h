#ifndef SIGNALWIDGET_H
#define SIGNALWIDGET_H

#include <QObject>
#include <QWidget>

#include "deviceprotocols.h"

class Driver;
class TestResultData;

/*!
 * \brief Базовый класс виджета сигналов SignalWidget class
 */
class SignalWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SignalWidget(Driver *driver, const QString channelId, QWidget *parent = nullptr);

//    virtual QString signalType() = 0;

    /*!
     * \brief Запоминает указатель на объект, предназначенный для записи данных в БД
     * \param trd - указатель на объект
     */
    virtual void assignTestResultDataObject(TestResultData *trd) {m_trd = trd;}

    /*!
     * \brief Начало новой пробы
     */
    virtual void newProbe() = 0;

    /*!
     * \brief Прерывание пробы
     */
    virtual void abortProbe() = 0;

    /*!
     * \brief Получение данных от устройства
     * \param data - данные
     */
    virtual void getData(DeviceProtocols::DeviceData *data) = 0;
    virtual void record(DeviceProtocols::DeviceData *data) = 0;

    virtual void setFrequency(const int frequency) = 0;

    QString channelId() {return m_channelId;}

    Driver* driver() {return m_driver;}

signals:

public slots:

protected:
    TestResultData* objTestResultData() {return m_trd;}

private:
    Driver *m_driver;
    TestResultData *m_trd {nullptr};
    QString m_channelId {""};
};

#endif // SIGNALWIDGET_H
