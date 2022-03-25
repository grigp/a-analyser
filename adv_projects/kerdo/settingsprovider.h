#ifndef SETTINGSPROVIDER_H
#define SETTINGSPROVIDER_H

#include <QVariant>
#include <QString>

namespace SettingsProvider
{
/*!
     * \brief Возвращает значение из реестра из раздела копии приложения
     * \param group - Название группы
     * \param param - Название параметра
     * \param defValue - Значение по умолчанию
     */
    QVariant valueFromRegAppCopy(const QString &group, const QString &param, const QVariant &defValue = QVariant());

    /*!
     * \brief Сохраняет значение в реестр в раздел копии приложения
     * \param group - Название группы
     * \param param - Название параметра
     * \param value - Значение параметра
     */
    void setValueToRegAppCopy(const QString &group, const QString &param, const QVariant &value);

}

/*!
 * \brief Класс значения параметра SettingsValue class
 * Предотвращает запись параметра в хранилище до его чтения
 * Сначала прочитали в конструкторе, затем можем записать
 */
class SettingsValue
{
public:
    /*!
     * \brief Создает экземпляр параметра и читает его из хранилища
     * \param group - раздел хранилища
     * \param param - название параметра
     * \param defValue - значение по умолчанию
     */
    explicit SettingsValue(const QString &group, const QString &param, const QVariant &defValue = QVariant());

    /*!
     * \brief Возвращает значение параметра
     */
    QVariant value() {return m_value;}
    /*!
     * \brief Устанавливает значение параметра и записывает его в хранилище
     * \param value - значение
     */
    void set(const QVariant& value);

private:
    QVariant m_value;
    QString m_group;
    QString m_param;
};

#endif // SETTINGSPROVIDER_H
