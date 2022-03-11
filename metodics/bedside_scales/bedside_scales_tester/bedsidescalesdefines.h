#ifndef BEDSIDESCALESDEFINES_H
#define BEDSIDESCALESDEFINES_H

#include "QCoreApplication"
#include <QMap>
#include <QString>

namespace BedsideScalesDefines
{

/*!
 * \brief Режимы работы методики The Mode enum
 */
enum Mode
{
      bsmTester = 0      ///< Тестер
    , bsmScales          ///< Весы
    , bsmSleepResearch   ///< Контроль сна
};

///< По режиму получить его uid
static QMap<Mode, QString> modeUid {
    std::pair<Mode, QString> (bsmTester, "tester")
  , std::pair<Mode, QString> (bsmScales, "scales")
  , std::pair<Mode, QString> (bsmSleepResearch, "sleep_research")
};

///< Получить код режима по его uid
static QMap<QString, Mode> modeCode {
    std::pair<QString, Mode> ("tester", bsmTester)
  , std::pair<QString, Mode> ("scales", bsmScales)
  , std::pair<QString, Mode> ("sleep_research", bsmSleepResearch)
};

///< По режиму получить его name
static QMap<Mode, QString> modeName {
    std::pair<Mode, QString> (bsmTester, QCoreApplication::tr("Тестировние"))
  , std::pair<Mode, QString> (bsmScales, QCoreApplication::tr("Весы"))
  , std::pair<Mode, QString> (bsmSleepResearch, QCoreApplication::tr("Исследования сна"))
};

}

#endif // BEDSIDESCALESDEFINES_H
