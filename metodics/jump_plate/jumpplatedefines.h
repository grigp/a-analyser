#ifndef JUMPPLATEDEFINES_H
#define JUMPPLATEDEFINES_H

#include <QMap>

namespace JumpPlateDefines
{
/*!
 * \brief Вид окончания записи в тесте TestFinishKind enum
 */
enum TestFinishKind
{
      tfkQuantity = 0   ///< Кол-во событий
    , tfkFixedTime      ///< Заданное время
    , tfkCommand        ///< По команде оператора
};

static QMap<QString, TestFinishKind> TestFinishKindIndex {
    std::pair<QString, TestFinishKind> ("quantity", tfkQuantity)
  , std::pair<QString, TestFinishKind> ("fixed_time", tfkFixedTime)
  , std::pair<QString, TestFinishKind> ("command", tfkCommand)
};

static QMap<TestFinishKind, QString> TestFinishKindValueName {
    std::pair<TestFinishKind, QString> (tfkQuantity, "quantity")
  , std::pair<TestFinishKind, QString> (tfkFixedTime, "fixed_time")
  , std::pair<TestFinishKind, QString> (tfkCommand, "command")
};

}

#endif // JUMPPLATEDEFINES_H
