#ifndef FACTORS_H
#define FACTORS_H

#include <QObject>

#include "factorsdefines.h"

/*!
 * \brief Базовый класс группы показателей, рассчитываемых одновременно, Factors class
 */
class Factors : public QObject
{
    Q_OBJECT
public:
    explicit Factors(QString &testUid, QObject *parent = nullptr);

    /*!
     * \brief Возвращает true, если показатели можно рассчитать для теста с заданным uid
     * uid теста задается в конструкторе
     */
    virtual bool isValid() = 0;

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

signals:

public slots:

protected:
    /*!
     * \brief Добавляет значение показателя в список
     * Показатели в список добавляют подклассы
     * \param uid - uid показателя
     * \param value - значение показателя
     */
    void addFactor(const QString &uid, const double value);

private:
    QString m_testUid;
    QList<FactorsDefines::FactorValue*> m_factors;
};

#endif // FACTORS_H
