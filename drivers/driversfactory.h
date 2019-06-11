#ifndef DRIVERSFACTORY_H
#define DRIVERSFACTORY_H

#include <QObject>

/*!
 * \brief Класс фабрики классов драйверов DriversFactory class
 */
class DriversFactory : public QObject
{
    Q_OBJECT
public:
    explicit DriversFactory(QObject *parent = nullptr);

signals:

public slots:
};

#endif // DRIVERSFACTORY_H
