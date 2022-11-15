#ifndef SIGNALTRANSFORMFACTORY_H
#define SIGNALTRANSFORMFACTORY_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>

class SignalTransformer;
class SignalTransformerParamsWidget;

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
     * \param params - параметры преобразователя
     */
    void transform(const int idx, QVector<double> &buffer, const QJsonObject& params) const;

    /*!
     * \brief Преобразование сигнала
     * \param uid - идентификатор преобразователя
     * \param buffer - преобразуемый сигнал
     * \param params - параметры преобразователя
     */
    void transform(const QString uid, QVector<double> &buffer, const QJsonObject& params) const;

    /*!
     * \brief Создает виджет редактирования параметров преобразователя и возвращает указатель на него
     * \param idx - индекс в списке
     */
    SignalTransformerParamsWidget* getParamsWidget(const int idx) const;

    /*!
     * \brief Создает виджет редактирования параметров преобразователя и возвращает указатель на него
     * \param uid - идентификатор преобразователя
     */
    SignalTransformerParamsWidget* getParamsWidget(const QString uid) const;

    /*!
     * \brief Запомнить схему преобразований
     * \param sheme - Схема преобразований
     */
    void rememberScheme(const QJsonArray& sheme);

    /*!
     * \brief Возвращает запомненную схему преобразований
     */
    QJsonArray getScheme();

    /*!
     * \brief Очищает запомненную схему преобразований
     */
    void clearSheme();

private:
    QList<SignalTransformer*> m_transformers;

    QJsonArray m_sheme {QJsonArray()};
};

#endif // SIGNALTRANSFORMFACTORY_H
