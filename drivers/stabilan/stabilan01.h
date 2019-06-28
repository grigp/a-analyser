#ifndef STABILAN01_H
#define STABILAN01_H

#include <QObject>

#include "driver.h"
#include "deviceprotocols.h"

namespace  {
    static const QString uid_stabilan01 = "{CEAD1669-1531-4B8E-9220-590C49BB310D}";
    static const QString name_stabilan01 = "Стабилан - 01";
}

/*!
 * \brief Класс драйвера стабилоанализатора Стабилан - 01 Stabilan01 class
 */
class Stabilan01 : public Driver, public DeviceProtocols::StabControl
{
    Q_OBJECT
public:
    explicit Stabilan01(QObject *parent = nullptr);

    static QString uid() {return uid_stabilan01;}
    static QString name() {return name_stabilan01;}

    /*!
     * \brief Устанавливает параметры драйвера
     * для использования при работе
     * \param params - параметры
     */
    void setParams(const QJsonObject &params) override;

    /*!
     * \brief Вызывает диалог редактирования параметров
     * \param params - редактируемые параметры
     * \return true, если диалог закончился командой сохранить параметры
     */
    bool editParams(QJsonObject &params) override;

    /*!
     * \brief Запуск передачи данных
     */
    void start() override;
    /*!
     * \brief Останов передачи данных
     */
    void stop() override;

    /*!
     * \brief Возвращает список протоколов
     */
    static QStringList getProtocols();

    /*!
     * \brief Возвращает список допустимых портов для драйвера
     */
    static QStringList getPorts();

    void calibrate() override;
    void zeroing() override;

};

#endif // STABILAN01_H
