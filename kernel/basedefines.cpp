#include "basedefines.h"

#include <QApplication>

#include "aanalyserapplication.h"

namespace  {
static QMap<BaseDefines::Directions, QString> DirectionValueFBName {
    std::pair<BaseDefines::Directions, QString> (BaseDefines::dirUp, QCoreApplication::tr("вперед"))
  , std::pair<BaseDefines::Directions, QString> (BaseDefines::dirRight, QCoreApplication::tr("вправо"))
  , std::pair<BaseDefines::Directions, QString> (BaseDefines::dirDown, QCoreApplication::tr("назад"))
  , std::pair<BaseDefines::Directions, QString> (BaseDefines::dirLeft, QCoreApplication::tr("влево"))
};

static QMap<BaseDefines::Directions, QString> DirectionValueFBName_en_US {
    std::pair<BaseDefines::Directions, QString> (BaseDefines::dirUp, QCoreApplication::tr("forward"))
  , std::pair<BaseDefines::Directions, QString> (BaseDefines::dirRight, QCoreApplication::tr("to the right"))
  , std::pair<BaseDefines::Directions, QString> (BaseDefines::dirDown, QCoreApplication::tr("backward"))
  , std::pair<BaseDefines::Directions, QString> (BaseDefines::dirLeft, QCoreApplication::tr("to the left"))
};

static QMap<BaseDefines::Directions, QString> DirectionValueUDName {
    std::pair<BaseDefines::Directions, QString> (BaseDefines::dirUp, QCoreApplication::tr("вверх"))
  , std::pair<BaseDefines::Directions, QString> (BaseDefines::dirRight, QCoreApplication::tr("вправо"))
  , std::pair<BaseDefines::Directions, QString> (BaseDefines::dirDown, QCoreApplication::tr("вниз"))
  , std::pair<BaseDefines::Directions, QString> (BaseDefines::dirLeft, QCoreApplication::tr("влево"))
};

static QMap<BaseDefines::Directions, QString> DirectionValueUDName_en_US {
    std::pair<BaseDefines::Directions, QString> (BaseDefines::dirUp, QCoreApplication::tr("up"))
  , std::pair<BaseDefines::Directions, QString> (BaseDefines::dirRight, QCoreApplication::tr("to the right"))
  , std::pair<BaseDefines::Directions, QString> (BaseDefines::dirDown, QCoreApplication::tr("down"))
  , std::pair<BaseDefines::Directions, QString> (BaseDefines::dirLeft, QCoreApplication::tr("to the left"))
};
}

QString BaseDefines::getDirectionValueFBName(const BaseDefines::Directions dir)
{
    if (static_cast<AAnalyserApplication*>(QApplication::instance())->languargeCode() == DataDefines::LANG_CODE_ENGUSA)
        return DirectionValueFBName_en_US.value(dir);
    else
        return DirectionValueFBName.value(dir);
}

QList<BaseDefines::Directions> BaseDefines::getDirectionValueFBKeys()
{
    return DirectionValueFBName.keys();
}

QString BaseDefines::getDirectionValueUDName(const BaseDefines::Directions dir)
{
    if (static_cast<AAnalyserApplication*>(QApplication::instance())->languargeCode() == DataDefines::LANG_CODE_ENGUSA)
        return DirectionValueUDName_en_US.value(dir);
    else
        return DirectionValueUDName.value(dir);
}

QList<BaseDefines::Directions> BaseDefines::getDirectionValueUDKeys()
{
    return DirectionValueUDName.keys();
}
