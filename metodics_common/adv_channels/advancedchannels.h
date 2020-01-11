#ifndef ADVANCEDCHANNELS_H
#define ADVANCEDCHANNELS_H

#include <QWidget>
#include <QStandardItemModel>
#include <QItemSelection>

#include "deviceprotocols.h"

namespace Ui {
class AdvancedChannels;
}

class Driver;
class SignalWidget;
class TestResultData;

/*!
 * \brief Класс виджета дополнительных физиологических каналов AdvancedChannels class
 */
class AdvancedChannels : public QWidget
{
    Q_OBJECT

public:
    explicit AdvancedChannels(QWidget *parent = 0);
    ~AdvancedChannels();

    /*!
     * \brief Роли для дерева каналов драйвера Roles enum
     */
    enum Roles
    {
          WidgetRole = Qt::UserRole + 1   ///< Указатель на виджет. Содержит SignalWidget*
        , ChannelIdRole                   ///< Идентификатор канала. Содержит QString
    };

    /*!
     * \brief Запоминает указатель на драйвер
     * Анализирует драйвер на предмет передаваемых каналов, создает для них виджеты, подключается к передаче данных от драйвера
     * \param driver - указатель на драйвер
     * \param trd - указатель на объект, пишущий данные в БД
     */
    void assignDriver(Driver* driver, TestResultData *trd);

    /*!
     * \brief Получение данных от устройства
     * \param data - данные
     */
    void getData(DeviceProtocols::DeviceData *data);

    /*!
     * \brief Начало новой пробы
     */
    void newProbe();

    /*!
     * \brief Прерывание пробы
     */
    void abortProbe();

    void record(DeviceProtocols::DeviceData *data);

    /*!
     * \brief Управляет допустимостью элементов на панелях
     */
    void enabledControls(const bool enabled);

    /*!
     * \brief Устанавливает канал, записываемый всегда
     * \param channelId - идентификатор канала
     */
    void setAllwaysRecordingChannel(const QString &channelId);

private slots:
    void on_selectIndex(QModelIndex index);
    void on_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void splitterMoved(int pos, int index);

private:
    Ui::AdvancedChannels *ui;

    /*!
     * \brief Создает виджет для канала по идентификатору канала
     */
    SignalWidget* createWidget(Driver* driver, const QString channelId);

    /*!
     * \brief Выделяет первый канал в списке
     */
    void selectFirstChannel();

    Driver *m_driver;                  ///< Указатель на драйвер
    QStandardItemModel m_mdlDrvChan;   ///< Модель каналов драйвера
};

#endif // ADVANCEDCHANNELS_H
