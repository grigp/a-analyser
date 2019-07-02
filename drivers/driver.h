#ifndef DRIVER_H
#define DRIVER_H

#include <QObject>
#include <QStringList>

/*!
 * \brief Базовый класс драйвера Driver class
 */
class Driver : public QObject
{
    Q_OBJECT
public:
    explicit Driver(QObject *parent = nullptr);

    /*!
     * \brief Устанавливает параметры драйвера
     * для использования при работе
     * \param params - параметры
     */
    virtual void setParams(const QJsonObject &params) = 0;

    /*!
     * \brief Вызывает диалог редактирования параметров
     * \param params - редактируемые параметры
     * \return true, если диалог закончился командой сохранить параметры
     */
//    virtual bool editParams(QJsonObject &params) = 0;

    /*!
     * \brief Запуск передачи данных
     */
    virtual void start() = 0;
    /*!
     * \brief Останов передачи данных
     */
    virtual void stop() = 0;


signals:

public slots:
};

#endif // DRIVER_H
