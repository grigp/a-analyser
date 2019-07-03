#ifndef JUMPPLATE_H
#define JUMPPLATE_H

#include <QObject>
#include "driver.h"
#include "deviceprotocols.h"

namespace  {
    static const QString uid_jumpPlate = "{ACC7467D-6600-4808-B372-EDC2069EBF5E}";
    static const QString name_jumpPlate = "Прыжковая платформа";
}

/*!
 * \brief Класс драйвера прыжковой платформы JumpPlate class
 */
class JumpPlate : public Driver, public DeviceProtocols::JumpPlateControl
{
    Q_OBJECT
public:
    explicit JumpPlate(QObject *parent = nullptr);

    static QString uid() {return uid_jumpPlate;}
    static QString name() {return name_jumpPlate;}

    /*!
     * \brief Перекрытые методы, возвращающие uid и name драйверов
     * uid и name можно использовать, как методы класса, а эти - только с экземпляром
     */
    QString driverUid() const override {return uid();}
    QString driverName() const override {return name();}

    /*!
     * \brief Устанавливает параметры драйвера
     * для использования при работе
     * \param params - параметры
     * \param port - порт
     */
    void setParams(const DeviceProtocols::Ports port, const QJsonObject &params) override;

    /*!
     * \brief Вызывает диалог редактирования параметров
     * \param params - редактируемые параметры
     * \return true, если диалог закончился командой сохранить параметры
     */
    static bool editParams(QJsonObject &params);

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
     * \return
     */
    static QStringList getProtocols();

    /*!
     * \brief Возвращает список допустимых портов для драйвера
     */
    static QList<DeviceProtocols::Ports> getPorts();

    void calibrate() override;
};

#endif // JUMPPLATE_H
