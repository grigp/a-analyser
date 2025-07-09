#include "basedefines.h"

#include <QApplication>

#include "aanalyserapplication.h"


QMap<BaseDefines::Directions, QString> BaseDefines::getDirectionValueFB()
{
    static QMap<BaseDefines::Directions, QString> DirectionValueFBName {
        std::pair<BaseDefines::Directions, QString> (BaseDefines::dirUp, QCoreApplication::tr("вперед"))
      , std::pair<BaseDefines::Directions, QString> (BaseDefines::dirRight, QCoreApplication::tr("вправо"))
      , std::pair<BaseDefines::Directions, QString> (BaseDefines::dirDown, QCoreApplication::tr("назад"))
      , std::pair<BaseDefines::Directions, QString> (BaseDefines::dirLeft, QCoreApplication::tr("влево"))
    };
    return DirectionValueFBName;
}

QMap<BaseDefines::Directions, QString> BaseDefines::getDirectionValueUD()
{
    static QMap<BaseDefines::Directions, QString> DirectionValueUDName {
        std::pair<BaseDefines::Directions, QString> (BaseDefines::dirUp, QCoreApplication::tr("вверх"))
      , std::pair<BaseDefines::Directions, QString> (BaseDefines::dirRight, QCoreApplication::tr("вправо"))
      , std::pair<BaseDefines::Directions, QString> (BaseDefines::dirDown, QCoreApplication::tr("вниз"))
      , std::pair<BaseDefines::Directions, QString> (BaseDefines::dirLeft, QCoreApplication::tr("влево"))
    };
    return DirectionValueUDName;
}
