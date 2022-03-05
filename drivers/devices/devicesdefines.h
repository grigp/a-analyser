#ifndef DEVICESDEFINES_H
#define DEVICESDEFINES_H

#include <QString>
#include <QCoreApplication>

namespace  DevicesDefines
{
    static const QString uid_stabilan01 = "{CEAD1669-1531-4B8E-9220-590C49BB310D}";
    static const QString uid_jumpPlate = "{ACC7467D-6600-4808-B372-EDC2069EBF5E}";
    static const QString uid_bedsidescales01 = "{53243842-E302-4F2F-ABE6-D628AACE833E}";


    static const QString name_stabilan01 = QCoreApplication::tr("Стабилан - 01");
    static const QString name_jumpPlate = QCoreApplication::tr("Прыжковая платформа");
    static const QString name_bedsidescales01 = QCoreApplication::tr("Прикроватные весы");
}


#endif // DEVICESDEFINES_H
