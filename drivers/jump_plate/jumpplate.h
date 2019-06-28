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
     * \return
     */
    static QStringList getProtocols();

    /*!
     * \brief Возвращает список допустимых портов для драйвера
     */
    static QStringList getPorts();

    void calibrate() override;
};

#endif // JUMPPLATE_H
