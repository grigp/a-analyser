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
    return nameAsConst();
}

QString StabReactTrainFactors::nameAsConst()
{
    return QCoreApplication::tr("Показатели тренажера статокинетической реакции");
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
            registerGroup(StabReactTrainFactorsDefines::GroupUid, nameAsConst());

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::LatentAvrUid, StabReactTrainFactorsDefines::GroupUid,
                           QCoreApplication::tr("Латентный период средний"), QCoreApplication::tr("LA"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::LatentUUid, StabReactTrainFactorsDefines::GroupUid,
                           QCoreApplication::tr("Латентный период вперед"), QCoreApplication::tr("LU"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::LatentURUid, StabReactTrainFactorsDefines::GroupUid,
                           QCoreApplication::tr("Латентный период вперед вправо"), QCoreApplication::tr("LUR"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::LatentRUid, StabReactTrainFactorsDefines::GroupUid,
                           QCoreApplication::tr("Латентный период вправо"), QCoreApplication::tr("LR"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::LatentDRUid, StabReactTrainFactorsDefines::GroupUid,
                           QCoreApplication::tr("Латентный период назад вправо"), QCoreApplication::tr("LDR"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::LatentDUid, StabReactTrainFactorsDefines::GroupUid,
                           QCoreApplication::tr("Латентный период назад"), QCoreApplication::tr("LD"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::LatentDLUid, StabReactTrainFactorsDefines::GroupUid,
                           QCoreApplication::tr("Латентный период назад влево"), QCoreApplication::tr("LDL"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::LatentLUid, StabReactTrainFactorsDefines::GroupUid,
                           QCoreApplication::tr("Латентный период влево"), QCoreApplication::tr("LL"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::LatentULUid, StabReactTrainFactorsDefines::GroupUid,
                           QCoreApplication::tr("Латентный период вперед влево"), QCoreApplication::tr("LUL"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::TimeAvrUid, StabReactTrainFactorsDefines::GroupUid,
                           QCoreApplication::tr("Время реакции среднее"), QCoreApplication::tr("TA"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::TimeUUid, StabReactTrainFactorsDefines::GroupUid,
                           QCoreApplication::tr("Время реакции вперед"), QCoreApplication::tr("TU"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::TimeURUid, StabReactTrainFactorsDefines::GroupUid,
                           QCoreApplication::tr("Время реакции вперед вправо"), QCoreApplication::tr("TUR"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::TimeRUid, StabReactTrainFactorsDefines::GroupUid,
                           QCoreApplication::tr("Время реакции вправо"), QCoreApplication::tr("TR"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::TimeDRUid, StabReactTrainFactorsDefines::GroupUid,
                           QCoreApplication::tr("Время реакции назад вправо"), QCoreApplication::tr("TDR"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::TimeDUid, StabReactTrainFactorsDefines::GroupUid,
                           QCoreApplication::tr("Время реакции назад"), QCoreApplication::tr("TD"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::TimeDLUid, StabReactTrainFactorsDefines::GroupUid,
                           QCoreApplication::tr("Время реакции назад влево"), QCoreApplication::tr("TDL"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::TimeLUid, StabReactTrainFactorsDefines::GroupUid,
                           QCoreApplication::tr("Время реакции влево"), QCoreApplication::tr("TL"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StabReactTrainFactorsDefines::TimeULUid, StabReactTrainFactorsDefines::GroupUid,
                           QCoreApplication::tr("Время реакции вперед влево"), QCoreApplication::tr("TUL"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
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
