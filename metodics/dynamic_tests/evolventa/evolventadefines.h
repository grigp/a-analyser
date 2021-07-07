#ifndef EVOLVENTADEFINES_H
#define EVOLVENTADEFINES_H

#include <QMap>
#include <QCoreApplication>


namespace EvolventaDefines
{

/*!
 * \brief Этапы эвольвенты Stage enum
 */
enum Stage
{
      stgUpwinding = 0   ///< Раскручивание
    , stgHold            ///< Удержание амплитуды
    , stgConvolution     ///< Сворачивание
};

static QMap<Stage, QString> StageValueName {
    std::pair<Stage, QString> (stgUpwinding, QCoreApplication::tr("раскрутка"))
  , std::pair<Stage, QString> (stgHold, QCoreApplication::tr("удержание"))
  , std::pair<Stage, QString> (stgConvolution, QCoreApplication::tr("сворачивание"))
};

}

#endif // EVOLVENTADEFINES_H
