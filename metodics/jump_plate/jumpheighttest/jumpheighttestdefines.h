#ifndef JUMPHEIGHTTESTDEFINES_H
#define JUMPHEIGHTTESTDEFINES_H

#include <QMap>

namespace JumpHeightTestDefines
{

/*!
 * \brief Вид окончания записи в тетсе TestFinishKind enum
 */
enum TestFinishKind
{
      tfkQuantity = 0   ///< Кол-во событий
    , tfkFixedTime      ///< Заданное время
};

/*!
 * \brief Вид стратегии при проведении Strategy enum
 */
enum Strategy
{
      jhsMaxHeight = 0     ///< Максимальная высота прыжка
    , jhsMinContactTime    ///< Минимальное время на платформе
};

static QMap<QString, TestFinishKind> TestFinishKindIndex {
    std::pair<QString, TestFinishKind> ("quantity", tfkQuantity)
  , std::pair<QString, TestFinishKind> ("fixed_time", tfkFixedTime)
};

static QMap<TestFinishKind, QString> TestFinishKindValueName {
    std::pair<TestFinishKind, QString> (tfkQuantity, "quantity")
  , std::pair<TestFinishKind, QString> (tfkFixedTime, "fixed_time")
};

static QMap<QString, Strategy> StrategyIndex {
    std::pair<QString, Strategy> ("height", jhsMaxHeight)
  , std::pair<QString, Strategy> ("min_contact_time", jhsMinContactTime)
};

static QMap<Strategy, QString> StrategyValueName {
    std::pair<Strategy, QString> (jhsMaxHeight, "height")
  , std::pair<Strategy, QString> (jhsMinContactTime, "min_contact_time")
};

}

#endif // JUMPHEIGHTTESTDEFINES_H
