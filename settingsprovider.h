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

#endif // SETTINGSPROVIDER_H
