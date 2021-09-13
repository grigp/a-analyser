#include "settingsprovider.h"

#include "datadefines.h"

#include <QSettings>
#include <QApplication>
#include <QDebug>

QVariant SettingsProvider::valueFromRegAppCopy(const QString &group, const QString &param, const QVariant &defValue)
{
    QSettings set(QApplication::instance()->organizationName(),
                  QApplication::instance()->applicationName());
    if (group != "")
        set.beginGroup(DataDefines::appCopyUid() + "/" + group);
    else
        set.beginGroup(DataDefines::appCopyUid());
    auto retval = set.value(param, defValue);
    set.endGroup();
    return retval;
}

void SettingsProvider::setValueToRegAppCopy(const QString &group, const QString &param, const QVariant &value)
{
    QSettings set(QApplication::instance()->organizationName(),
                  QApplication::instance()->applicationName());
    if (group != "")
        set.beginGroup(DataDefines::appCopyUid() + "/" + group);
    else
        set.beginGroup(DataDefines::appCopyUid());
    set.setValue(param, value);
    set.endGroup();
}



SettingsValue::SettingsValue(const QString &group, const QString &param, const QVariant &defValue)
{
    m_group = group;
    m_param = param;
    m_value = SettingsProvider::valueFromRegAppCopy(group, param, defValue);
}

void SettingsValue::set(const QVariant &value)
{
    m_value = value;
    SettingsProvider::setValueToRegAppCopy(m_group, m_param, value);
}
