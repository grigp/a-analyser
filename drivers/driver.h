#ifndef DRIVER_H
#define DRIVER_H

#include <QObject>

/*!
 * \brief Базовый класс драйвера Driver class
 */
class Driver : public QObject
{
    Q_OBJECT
public:
    explicit Driver(QObject *parent = nullptr);

    virtual QString uid() const = 0;
    virtual QString name() const = 0;

    /*!
     * \brief Устанавливает параметры драйвера
     * для использования при работе
     * \param params - параметры
     */
    virtual void setParams(const QJsonObject &params) = 0;

    /*!
     * \brief Вызывает диалог редактирования параметров
     */
    virtual void editParams() = 0;

    /*!
     * \brief Запуск передачи данных
     */
    virtual void start() = 0;
    /*!
     * \brief Останов передачи данных
     */
    virtual void stop() = 0;

    /*!
     * \brief Возвращает список протоколов
     * \return
     */
    virtual QList<int> getProtocols() = 0;

signals:

public slots:
};

#endif // DRIVER_H
