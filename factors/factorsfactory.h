#ifndef FACTORSFACTORY_H
#define FACTORSFACTORY_H

#include <QObject>
#include <QMap>

#include "factorsdefines.h"

class TestFactorsCreator;
class ProbeFactorsCreator;
class ChannelFactorsCreator;

/*!
 * \brief Класс фабрики классов показателей FactorsFactory class
 */
class FactorsFactory : public QObject
{
    Q_OBJECT
public:
    explicit FactorsFactory(QObject *parent = nullptr);

    /*!
     * \brief Возвращает информацию о показателе
     * \param uid - uid показателя
     */
    FactorsDefines::FactorInfo getFactorInfo(const QString &uid) const;

    /*!
     * \brief Возвращает название группы показателей по uid группы
     * \param groupUid - uid группы показателей
     */
    QString getGroupName(const QString &groupUid) const;

    /*!
     * \brief Регистрирует показатель и информацию о нем.
     * Этот метод вызывают факторсгруппы
     */
    void registerFactor(const QString &uid, const QString &groupUid,
                        const QString &name, const QString &shortName,
                        const QString &measure, const int format,
                        const double multiplier, const FactorsDefines::NormSide side, const int agingPeriod);

    /*!
     * \brief Регистрирует группу показателей
     * \param uid - uid группы
     * \param name - название группы
     */
    void registerGroup(const QString &uid, const QString &name);

signals:

public slots:

private:
    QMap<QString, FactorsDefines::FactorInfo> m_factors;
    QMap<QString, QString> m_groups;

    QMap<QString, TestFactorsCreator*> m_creatorsTest;
    QMap<QString, ProbeFactorsCreator*> m_creatorsProbe;
    QMap<QString, ChannelFactorsCreator*> m_creatorsChannel;
};

#endif // FACTORSFACTORY_H
