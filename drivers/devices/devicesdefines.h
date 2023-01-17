#ifndef DEVICESDEFINES_H
#define DEVICESDEFINES_H

#include <QString>
#include <QCoreApplication>

namespace  DevicesDefines
{
    static const QString uid_stabilan01 = "{CEAD1669-1531-4B8E-9220-590C49BB310D}";
    static const QString uid_amed_platform01 = "{957AF988-AA2B-4C2F-B837-F433FA104DE4}";
    static const QString uid_jumpPlate = "{ACC7467D-6600-4808-B372-EDC2069EBF5E}";
    static const QString uid_bedsidescales01 = "{53243842-E302-4F2F-ABE6-D628AACE833E}";
    static const QString uid_bilateral = "{31ED3CFC-6018-4469-AB38-2CF75C17A5C1}";


    static const QString name_stabilan01 = QCoreApplication::tr("Стабилан - 01");
    static const QString name_amed_platform01 = QCoreApplication::tr("Стабилоанализатор А-Мед");
    static const QString name_jumpPlate = QCoreApplication::tr("Прыжковая платформа");
    static const QString name_bedsidescales01 = QCoreApplication::tr("Прикроватные весы");
    static const QString name_bilateral = QCoreApplication::tr("Билатеральный стабилограф");
}


#endif // DEVICESDEFINES_H
