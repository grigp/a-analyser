#ifndef MULTIFACTOR_H
#define MULTIFACTOR_H

#include <QObject>

#include "factorsdefines.h"

/*!
 * \brief Базовый класс группы показателей, рассчитываемых одновременно, MultiFactor class
 */
class MultiFactor : public QObject
{
    Q_OBJECT
public:
    explicit MultiFactor(QObject *parent = nullptr);

    /*!
     * \brief Возвращает uid группы показателей
     */
    virtual QString uid() const = 0;

    /*!
     * \brief Возвращает название группы показателей
     */
    virtual QString name() const = 0;

    /*!
     * \brief Возвращает true, если показатели можно рассчитать для теста с заданным uid
     * uid теста задается в конструкторе
     */
    virtual bool isValid() const = 0;

    /*!
     * \brief Рассчитывает показатели для теста с заданным uid
     * uid теста задается в конструкторе
     */
    virtual void calculate() = 0;

    /*!
     * \brief Возвращает кол-во показателей в группе
     */
    int size() const {return m_factors.size();}

    /*!
     * \brief Возвращает uid показателя по номеру в списке
     * \param id - номер показателя в списке
     */
    QString factorUid(const int id) const;

    /*!
     * \brief Возвращает значение показателя по номеру в списке
     * \param id - номер показателя в списке
     */
    double factorValue(const int id) const;

    /*!
     * \brief Возвращает значение показателя по uid показателя
     * \param uid - uid показателя
     */
    double factorValue(const QString &uid) const;

    /*!
     * \brief Возвращает форматированное значение показателя в строковом виде по uid показателя
     * \param uid - uid показателя
     */
    QString factorValueFormatted(const QString &uid) const;

    /*!
     * \brief Проверяет наличие показателя в списке
     * \param uid - uid показателя
     * \return true, если показатель с таким uid имеется
     */
    bool factorExists(const QString &uid) const;

signals:

public slots:

    /*!
     * \brief Добавляет значение показателя в список
     * Показатели в список добавляют подклассы
     * \param uid - uid показателя
     * \param value - значение показателя
     */
    void addFactor(const QString &uid, const double value);

private:
    QList<FactorsDefines::FactorValue*> m_factors;
};


///<---------------------------------------------------------------------------
/*!
 * \brief Группа показателей уровня теста TestMultifactor class
 */
class TestMultifactor : public MultiFactor
{
public:
    explicit TestMultifactor(const QString &testUid, QObject *parent = nullptr);

protected:
    /*!
     * \brief Возвращает uid теста. Для использования в подклассах
     */
    QString testUid() const {return m_testUid;}

private:
    QString m_testUid;
};

///<---------------------------------------------------------------------------
/*!
 * \brief Группа показателей уровня пробы ProbeMultifactor class
 */
class ProbeMultifactor : public TestMultifactor
{
public:
    explicit ProbeMultifactor(const QString &testUid, const QString &probeUid, QObject *parent = nullptr);

protected:
    /*!
     * \brief Возвращает uid пробы. Для использования в подклассах
     */
    QString probeUid() const {return m_probeUid;}

private:
    QString m_probeUid;
};

///<---------------------------------------------------------------------------
/*!
 * \brief Группа показателей уровня канала ChannelMultifactor class
 */
class ChannelMultifactor : public ProbeMultifactor
{
public:
    explicit ChannelMultifactor(const QString &testUid,
                                const QString &probeUid,
                                const QString &channelId,
                                QObject *parent = nullptr);

protected:
    /*!
     * \brief Возвращает id канала. Для использования в подклассах
     */
    QString channelId() const {return m_channelId;}

private:
    QString m_channelId;
};



///<---------------------------------------------------------------------------
/*!
 * \brief Базовый класс для создания мультифакторов TestFactorsCreator class
 */
class TestFactorsCreator
{
public:
    TestFactorsCreator() {}
    virtual TestMultifactor* createFactors(const QString &testUid,
                                           QObject *parent = nullptr) = 0;

    virtual bool isValid(const QString &testUid) const = 0;
};

/*!
 * \brief Базовый класс для создания мультифакторов ProbeFactorsCreator class
 */
class ProbeFactorsCreator //: public TestFactorsCreator
{
public:
    ProbeFactorsCreator() {}
    virtual ProbeMultifactor* createFactors(const QString &testUid,
                                            const QString &probeUid,
                                            QObject *parent = nullptr) = 0;

    virtual bool isValid(const QString &testUid,
                         const QString &probeUid) const = 0;
};

/*!
 * \brief Базовый класс для создания мультифакторов ChannelFactorsCreator class
 */
class ChannelFactorsCreator //: public ProbeFactorsCreator
{
public:
    ChannelFactorsCreator() {}
    virtual ChannelMultifactor* createFactors(const QString &testUid,
                                              const QString &probeUid,
                                              const QString &channelId,
                                              QObject *parent = nullptr) = 0;

    virtual bool isValid(const QString &testUid,
                         const QString &probeUid,
                         const QString &channelId) const = 0;
};


#endif // MULTIFACTOR_H
