#include "evolventadefines.h"


EvolventaDefines::StageValueName::StageValueName(QObject *parent)
    : QObject (parent)
    , m_map({
            std::pair<Stage, QString> (stgUpwinding, QCoreApplication::tr("раскрутка"))
          , std::pair<Stage, QString> (stgHold, QCoreApplication::tr("удержание"))
          , std::pair<Stage, QString> (stgConvolution, QCoreApplication::tr("сворачивание"))
            })
{

}

QString EvolventaDefines::StageValueName::value(const EvolventaDefines::Stage stg)
{
    return m_map.value(stg);
}
