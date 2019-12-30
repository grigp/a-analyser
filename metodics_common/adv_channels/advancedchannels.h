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
     */
    void assignDriver(Driver* driver);

    /*!
     * \brief Получение данных от устройства
     * \param data - данные
     */
    void getData(DeviceProtocols::DeviceData *data);

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
