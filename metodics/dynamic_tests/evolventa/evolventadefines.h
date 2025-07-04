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

static QMap<Stage, QString> StageValueNameMap {
    std::pair<Stage, QString> (stgUpwinding, QCoreApplication::tr("раскрутка"))
  , std::pair<Stage, QString> (stgHold, QCoreApplication::tr("удержание"))
  , std::pair<Stage, QString> (stgConvolution, QCoreApplication::tr("сворачивание"))
};

class StageValueName : public QObject
{
public:
    StageValueName(QObject *parent = nullptr);
    QString value(const Stage stg);
private:
    QMap<Stage, QString> m_map;
};

}

#endif // EVOLVENTADEFINES_H
