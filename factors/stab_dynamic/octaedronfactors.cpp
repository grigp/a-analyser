#include "octaedronfactors.h"

#include <QDebug>

#include "aanalyserapplication.h"
#include "dataprovider.h"
#include "channelsdefines.h"
#include "octaedronresultdata.h"
#include "stabilogram.h"

OctaedronFactors::OctaedronFactors(const QString &testUid,
                                   const QString &probeUid,
                                   QObject *parent)
    : ProbeMultifactor (testUid, probeUid, parent)
{
    if (isValid())
        calculate();
}

QString OctaedronFactors::uid() const
{
    return OctaedronFactorsDefines::GroupUid;
}

QString OctaedronFactors::name() const
{
    return nameAsConst();
}

QString OctaedronFactors::nameAsConst()
{
    return QCoreApplication::tr("Показатели тренажера \"Октаэдр\"");
}

bool OctaedronFactors::isValid() const
{
    return isValid(testUid(), probeUid());
}

bool OctaedronFactors::isValid(const QString &testUid, const QString &probeUid)
{
    Q_UNUSED(testUid);
    auto de = DataProvider::channelExists(probeUid, ChannelsDefines::chanStab);
    auto se = DataProvider::channelExists(probeUid, ChannelsDefines::chanOctaedronResult);
    return de && se;
}

void OctaedronFactors::calculate()
{
    getEventLablels();
    calculateBaseFactors();

    double va = 0;
    for (int i = 0; i < 8; ++i)
        va = va + m_values[i];
    va = va / 8;

    addFactor(FactorsDefines::CommonFactors::SuccessUid, va);
    addFactor(OctaedronFactorsDefines::QualityUUid, m_values[0]);
    addFactor(OctaedronFactorsDefines::QualityULUid, m_values[1]);
    addFactor(OctaedronFactorsDefines::QualityLUid, m_values[2]);
    addFactor(OctaedronFactorsDefines::QualityDLUid, m_values[3]);
    addFactor(OctaedronFactorsDefines::QualityDUid, m_values[4]);
    addFactor(OctaedronFactorsDefines::QualityDRUid, m_values[5]);
    addFactor(OctaedronFactorsDefines::QualityRUid, m_values[6]);
    addFactor(OctaedronFactorsDefines::QualityURUid, m_values[7]);
}

void OctaedronFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(OctaedronFactorsDefines::GroupUid, nameAsConst());

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(FactorsDefines::CommonFactors::SuccessUid,
                           OctaedronFactorsDefines::GroupUid,
                           QCoreApplication::tr("Успешность выполнения задания"), QCoreApplication::tr("Успешность"), QCoreApplication::tr("%"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(OctaedronFactorsDefines::QualityUUid, OctaedronFactorsDefines::GroupUid,
                           QCoreApplication::tr("Отклонение вперед"), QCoreApplication::tr("QU"), QCoreApplication::tr("%"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(OctaedronFactorsDefines::QualityULUid, OctaedronFactorsDefines::GroupUid,
                           QCoreApplication::tr("Отклонение вперед влево"), QCoreApplication::tr("QUL"), QCoreApplication::tr("%"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(OctaedronFactorsDefines::QualityLUid, OctaedronFactorsDefines::GroupUid,
                           QCoreApplication::tr("Отклонение влево"), QCoreApplication::tr("QL"), QCoreApplication::tr("%"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(OctaedronFactorsDefines::QualityDLUid, OctaedronFactorsDefines::GroupUid,
                           QCoreApplication::tr("Отклонение назад влево"), QCoreApplication::tr("QDL"), QCoreApplication::tr("%"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(OctaedronFactorsDefines::QualityDUid, OctaedronFactorsDefines::GroupUid,
                           QCoreApplication::tr("Отклонение назад"), QCoreApplication::tr("QD"), QCoreApplication::tr("%"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(OctaedronFactorsDefines::QualityDRUid, OctaedronFactorsDefines::GroupUid,
                           QCoreApplication::tr("Отклонение назад вправо"), QCoreApplication::tr("QDR"), QCoreApplication::tr("%"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(OctaedronFactorsDefines::QualityRUid, OctaedronFactorsDefines::GroupUid,
                           QCoreApplication::tr("Отклонение вправо"), QCoreApplication::tr("QR"), QCoreApplication::tr("%"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(OctaedronFactorsDefines::QualityURUid, OctaedronFactorsDefines::GroupUid,
                           QCoreApplication::tr("Отклонение вперед вправо"), QCoreApplication::tr("QUR"), QCoreApplication::tr("%"), 0, 3, FactorsDefines::nsDual, 12);

}

int OctaedronFactors::stageTime() const
{
    if (m_resData)
        return m_resData->stageTime();
    return 5;
}

QString OctaedronFactors::circeRoundRuleMode() const
{
    if (m_resData)
        return m_resData->circeRoundRuleMode();
    return BaseDefines::CirceRoundRuleModeValueName.value(BaseDefines::crmRadial);
}

QString OctaedronFactors::directionMode() const
{
    if (m_resData)
        return m_resData->directionMode();
    return BaseDefines::DirectionModeValueName.value(BaseDefines::dmCounterClockwise);
}

int OctaedronFactors::radius() const
{
    if (m_resData)
        return m_resData->radius();
    return 50;
}

int OctaedronFactors::freq() const
{
    if (m_resData)
        return m_resData->freq();
    return 50;
}

int OctaedronFactors::diap() const
{
    if (m_resData)
        return m_resData->diap();
    return 128;
}

void OctaedronFactors::getEventLablels()
{
    QByteArray baData;
    if (DataProvider::getChannel(probeUid(), ChannelsDefines::chanOctaedronResult, baData))
        m_resData = new OctaedronResultData(baData);
}

void OctaedronFactors::calculateBaseFactors()
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
            double txp{0}, typ{0};
            m_resData->stage(i, code, begin, tx, ty);

            //! Код -1 - это всегда возврат при радиальном движении,
            //! а  при кольцевом всегда только первый этап подготовительный
            if (code > -1 || (getCRRM() == BaseDefines::crmCircle && i > 0))
            {
                //! Конечная точка этапа
                int end = begin + m_resData->stageTime() * m_resData->freq();
                if (begin <= stab.size() && end <= stab.size())
                {
                    //! По сигналу
                    double r = 0;
                    for (int j = begin; j < end; ++j)
                    {
                        double x = stab.value(0, j);
                        double y = stab.value(1, j);
                        r = r + sqrt(pow(tx - x, 2) + pow(ty - y, 2));
                    }
                    r = r / static_cast<double>(end - begin);
                    //! Максимальное расстояние для этапа между предыдущей и текущей точками
                    //! Первая точка никогда не учитывается!!!
                    double maxR = sqrt(pow(tx - txp, 2) + pow(ty - typ, 2));

                    //! Показатель для прохода
                    double v = 100 - r / maxR * 100;
                    if (v < 0) v = 0;
                    if (v > 100) v = 100;
                    m_values[code] = v;
                }
            }

            txp = tx;
            typ = ty;
        }
    }
}

BaseDefines::CirceRoundRuleMode OctaedronFactors::getCRRM()
{
    if (m_resData)
        return BaseDefines::CirceRoundRuleModeValueIndex.value(m_resData->circeRoundRuleMode());
    return BaseDefines::crmRadial;
}
