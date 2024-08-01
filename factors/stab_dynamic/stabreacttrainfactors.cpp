#include "stabreacttrainfactors.h"

#include <QDebug>

#include "aanalyserapplication.h"
#include "channelsdefines.h"
#include "dataprovider.h"
#include "octaedronresultdata.h"
#include "stabilogram.h"

StabReactTrainFactors::StabReactTrainFactors(const QString &testUid, const QString &probeUid, QObject *parent)
    : ProbeMultifactor (testUid, probeUid, parent)
{
    if (isValid())
        calculate();
}

QString StabReactTrainFactors::uid() const
{
    return StabReactTrainFactorsDefines::GroupUid;
}

QString StabReactTrainFactors::name() const
{
    return StabReactTrainFactorsDefines::GroupName;
}

bool StabReactTrainFactors::isValid() const
{
    return isValid(testUid(), probeUid());
}

bool StabReactTrainFactors::isValid(const QString &testUid, const QString &probeUid)
{
    Q_UNUSED(testUid);
    auto de = DataProvider::channelExists(probeUid, ChannelsDefines::chanStab);
    auto se = DataProvider::channelExists(probeUid, ChannelsDefines::chanOctaedronResult);
    return de && se;
}

void StabReactTrainFactors::calculate()
{
    getEventLablels();
    calculateBaseFactors();

    m_valLatentAvg = 0;
    m_valTimeAvg = 0;
    for (int i = 0; i < 8; ++i)
    {
        m_valLatentAvg += m_valLatent[i];
        m_valTimeAvg += m_valTime[i];
    }
    m_valLatentAvg /= 8;
    m_valTimeAvg /= 8;
    addFactor(StabReactTrainFactorsDefines::LatentAvrUid, m_valLatentAvg);
    addFactor(StabReactTrainFactorsDefines::LatentUUid, m_valLatent[0]);
    addFactor(StabReactTrainFactorsDefines::LatentURUid, m_valLatent[1]);
    addFactor(StabReactTrainFactorsDefines::LatentRUid, m_valLatent[2]);
    addFactor(StabReactTrainFactorsDefines::LatentDRUid, m_valLatent[3]);
    addFactor(StabReactTrainFactorsDefines::LatentDUid, m_valLatent[4]);
    addFactor(StabReactTrainFactorsDefines::LatentDLUid, m_valLatent[5]);
    addFactor(StabReactTrainFactorsDefines::LatentLUid, m_valLatent[6]);
    addFactor(StabReactTrainFactorsDefines::LatentULUid, m_valLatent[7]);

    addFactor(StabReactTrainFactorsDefines::TimeAvrUid, m_valTimeAvg);
    addFactor(StabReactTrainFactorsDefines::TimeUUid, m_valTime[0]);
    addFactor(StabReactTrainFactorsDefines::TimeURUid, m_valTime[1]);
    addFactor(StabReactTrainFactorsDefines::TimeRUid, m_valTime[2]);
    addFactor(StabReactTrainFactorsDefines::TimeDRUid, m_valTime[3]);
    addFactor(StabReactTrainFactorsDefines::TimeDUid, m_valTime[4]);
    addFactor(StabReactTrainFactorsDefines::TimeDLUid, m_valTime[5]);
    addFactor(StabReactTrainFactorsDefines::TimeLUid, m_valTime[6]);
    addFactor(StabReactTrainFactorsDefines::TimeULUid, m_valTime[7]);
}

void StabReactTrainFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(StabReactTrainFactorsDefines::GroupUid, StabReactTrainFactorsDefines::GroupName);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::LatentAvrUid, StabReactTrainFactorsDefines::GroupUid,
                           tr("Латентный период средний"), tr("LA"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::LatentUUid, StabReactTrainFactorsDefines::GroupUid,
                           tr("Латентный период вперед"), tr("LU"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::LatentURUid, StabReactTrainFactorsDefines::GroupUid,
                           tr("Латентный период вперед вправо"), tr("LUR"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::LatentRUid, StabReactTrainFactorsDefines::GroupUid,
                           tr("Латентный период вправо"), tr("LR"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::LatentDRUid, StabReactTrainFactorsDefines::GroupUid,
                           tr("Латентный период назад вправо"), tr("LDR"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::LatentDUid, StabReactTrainFactorsDefines::GroupUid,
                           tr("Латентный период назад"), tr("LD"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::LatentDLUid, StabReactTrainFactorsDefines::GroupUid,
                           tr("Латентный период назад влево"), tr("LDL"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::LatentLUid, StabReactTrainFactorsDefines::GroupUid,
                           tr("Латентный период влево"), tr("LL"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::LatentULUid, StabReactTrainFactorsDefines::GroupUid,
                           tr("Латентный период вперед влево"), tr("LUL"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::TimeAvrUid, StabReactTrainFactorsDefines::GroupUid,
                           tr("Время реакции среднее"), tr("TA"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::TimeUUid, StabReactTrainFactorsDefines::GroupUid,
                           tr("Время реакции вперед"), tr("TU"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::TimeURUid, StabReactTrainFactorsDefines::GroupUid,
                           tr("Время реакции вперед вправо"), tr("TUR"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::TimeRUid, StabReactTrainFactorsDefines::GroupUid,
                           tr("Время реакции вправо"), tr("TR"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::TimeDRUid, StabReactTrainFactorsDefines::GroupUid,
                           tr("Время реакции назад вправо"), tr("TDR"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::TimeDUid, StabReactTrainFactorsDefines::GroupUid,
                           tr("Время реакции назад"), tr("TD"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::TimeDLUid, StabReactTrainFactorsDefines::GroupUid,
                           tr("Время реакции назад влево"), tr("TDL"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::TimeLUid, StabReactTrainFactorsDefines::GroupUid,
                           tr("Время реакции влево"), tr("TL"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::TimeULUid, StabReactTrainFactorsDefines::GroupUid,
                           tr("Время реакции вперед влево"), tr("TUL"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
}

double StabReactTrainFactors::latent(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < 8);
    return m_valLatent[idx];
}

double StabReactTrainFactors::time(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < 8);
    return m_valTime[idx];
}

void StabReactTrainFactors::getEventLablels()
{
    QByteArray baData;
    if (DataProvider::getChannel(probeUid(), ChannelsDefines::chanOctaedronResult, baData))
        m_resData = new OctaedronResultData(baData);
}

void StabReactTrainFactors::calculateBaseFactors()
{
    QByteArray baStab;
    if (DataProvider::getChannel(probeUid(), ChannelsDefines::chanStab, baStab))
    {
        Stabilogram stab(baStab);

        //! По этапам теста
        for (int i = 0; i < m_resData->stagesCount(); ++i)
        {
            //! Параметры этапа
            int code{0}, begin{0};
            double tx{0}, ty{0};
//            double txp{0}, typ{0};
            int end {0};
            //! Берем конечную точку сначала, чтобы не плодить переменные без надобности
            m_resData->stage(i+1, code, end, tx, ty);
            m_resData->stage(i, code, begin, tx, ty);

            //! Код -1 - это всегда возврат при радиальном движении,
            //! а  при кольцевом всегда только первый этап подготовительный
            if (code > -1 || (getCRRM() == BaseDefines::crmCircle && i > 0))
            {
                if (begin <= stab.size() && end <= stab.size())
                {
                    //! Максимальное расстояние для этапа между предыдущей и текущей точками
                    //! Первая точка никогда не учитывается!!!
//                    double maxR = sqrt(pow(tx - txp, 2) + pow(ty - typ, 2));

                    //! МО первых 0.2 секунд
                    double mid = 0;
                    for (int j = begin; j < begin + stab.frequency() / 5; ++j)
                    {
                        double x = stab.value(0, j);
                        double y = stab.value(1, j);
                        double r = sqrt(pow(tx - x, 2) + pow(ty - y, 2));
                        mid += r;
                    }
                    mid /= (stab.frequency() / 5);

                    //! По сигналу
                    for (int j = begin; j < end; ++j)
                    {
                        double x = stab.value(0, j);
                        double y = stab.value(1, j);
                        double r = sqrt(pow(tx - x, 2) + pow(ty - y, 2));
                        if (fabs(r - mid) > 10)
                        {
                            //! Показатель латентного периода для прохода
                            m_valLatent[code] = static_cast<double>(j - begin) / stab.frequency();
                            break;
                        }
                    }

                    //! Показатель времени реакции для прохода
                    m_valTime[code] = static_cast<double>(end - begin) / stab.frequency();
                }
            }

//            txp = tx;
//            typ = ty;
        }
    }
}

BaseDefines::CirceRoundRuleMode StabReactTrainFactors::getCRRM()
{
    if (m_resData)
        return BaseDefines::CirceRoundRuleModeValueIndex.value(m_resData->circeRoundRuleMode());
    return BaseDefines::crmRadial;
}
