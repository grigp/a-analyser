#ifndef JUMPTESTDEFINES_H
#define JUMPTESTDEFINES_H

#include <QMap>
#include <QString>

namespace JumpTestDefines
{

static QMap<bool, QString> BusyValue
{
    std::pair<bool, QString> (true, "Занято")
  , std::pair<bool, QString> (false, "Пусто")
};

enum Methodic
{
      MetJumpHeight = 0
    , MetHopping
};

}

#endif // JUMPTESTDEFINES_H
