#ifndef SIGNALTRANSFORMFACTORY_H
#define SIGNALTRANSFORMFACTORY_H

#include <QObject>

class SignalTransformer;

/*!
 * \brief Класс фабрики классов преобразователей сигналов The SignalTransformFactory class
 */
class SignalTransformFactory : public QObject
{
    Q_OBJECT
public:
    explicit SignalTransformFactory(QObject *parent = nullptr);

    /*!
     * \brief Регистрирует преобразователь сигнала
     * \param st - указатель на экземпляр класса преобразователя сигнала
     */
    void registerTransformer(SignalTransformer* st);

    /*!
     * \brief Возвращает кол-во преобразователей
     */
    int count();

    /*!
     * \brief Возвращает uid преобразователя по индексу в списке
     * \param idx - индекс в списке
     */
    QString uid(const int idx) const;

    /*!
     * \brief Возвращает название преобразователя по индексу в списке
     * \param idx - индекс в списке
     */
    QString name(const int idx) const;

    /*!
     * \brief Преобразование сигнала
     * \param idx - индекс в списке
     * \param buffer - преобразуемый сигнал
     */
    void transform(const int idx, QVector<double> &buffer) const;

    /*!
     * \brief Преобразование сигнала
     * \param uid - идентификатор преобразователя
     * \param buffer - преобразуемый сигнал
     */
    void transform(const QString uid, QVector<double> &buffer) const;

private:
    QList<SignalTransformer*> m_transformers;
};

#endif // SIGNALTRANSFORMFACTORY_H
