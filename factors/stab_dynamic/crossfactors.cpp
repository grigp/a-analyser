#include "crossfactors.h"

#include "aanalyserapplication.h"
#include "datadefines.h"
#include "dataprovider.h"
#include "channelsdefines.h"
#include "stabilogram.h"
#include "crossresultdata.h"

#include <QDebug>

CrossFactors::CrossFactors(const QString &testUid,
                           const QString &probeUid,
                           QObject *parent)
    : ProbeMultifactor (testUid, probeUid, parent)
{
    if (isValid())
        calculate();
}

bool CrossFactors::isValid() const
{
    return isValid(testUid(), probeUid());
}

bool CrossFactors::isValid(const QString &testUid, const QString &probeUid)
{
    Q_UNUSED(testUid);
    auto de = DataProvider::channelExists(probeUid, ChannelsDefines::chanStab);
    auto se = DataProvider::channelExists(probeUid, ChannelsDefines::chanCrossResult);
    return de && se;

//    Q_UNUSED(probeUid);
//    DataDefines::TestInfo ti;
//    if (DataProvider::getTestInfo(testUid, ti))
//        foreach (auto probeUid, ti.probes)
//        {
//            auto de = DataProvider::channelExists(probeUid, ChannelsDefines::chanStab);
//            auto se = DataProvider::channelExists(probeUid, ChannelsDefines::chanCrossResult);
//            if (!se)
//                return false;
//        }
//    return ti.probes.size() > 1 && ti.probes.size() <= 5;
}

void CrossFactors::calculate()
{
    getEventLablels();
    fillBuffers();

    m_LUp = computeDistance(m_bufferUp, CrossDefines::dirUp);
    m_LDn = computeDistance(m_bufferDn, CrossDefines::dirDown);
    m_LRt = computeDistance(m_bufferRt, CrossDefines::dirRight);
    m_LLf = computeDistance(m_bufferLf, CrossDefines::dirLeft);
    m_SZone = (m_LUp * m_LRt + m_LRt * m_LDn + m_LDn * m_LLf + m_LLf * m_LUp) / 2;
    m_UD = m_LUp / m_LDn;
    m_RL = m_LRt / m_LLf;
    m_SF = (m_LUp + m_LDn) / (m_LRt + m_LLf);

    addFactor(CrossFactorsDefines::LUpUid, m_LUp);
    addFactor(CrossFactorsDefines::LDnUid, m_LDn);
    addFactor(CrossFactorsDefines::LRtUid, m_LRt);
    addFactor(CrossFactorsDefines::LLfUid, m_LLf);
    addFactor(CrossFactorsDefines::SZoneUid, m_SZone);
    addFactor(CrossFactorsDefines::UDUid, m_UD);
    addFactor(CrossFactorsDefines::RLUid, m_RL);
    addFactor(CrossFactorsDefines::SFUid, m_SF);
}

void CrossFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(CrossFactorsDefines::GroupUid, tr("Показатели теста на устойчивость"));

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(CrossFactorsDefines::LUpUid, CrossFactorsDefines::GroupUid,
                           tr("Отклонение вперед"), tr("LUp"), tr("мм"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(CrossFactorsDefines::LDnUid, CrossFactorsDefines::GroupUid,
                           tr("Отклонение назад"), tr("LDown"), tr("мм"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(CrossFactorsDefines::LRtUid, CrossFactorsDefines::GroupUid,
                           tr("Отклонение вправо"), tr("LRight"), tr("мм"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(CrossFactorsDefines::LLfUid, CrossFactorsDefines::GroupUid,
                           tr("Отклонение влево"), tr("LLeft"), tr("мм"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(CrossFactorsDefines::SZoneUid, CrossFactorsDefines::GroupUid,
                           tr("Площадь зоны перермещения"), tr("SZone"), tr("кв.мм"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(CrossFactorsDefines::UDUid, CrossFactorsDefines::GroupUid,
                           tr("Отношение вперед / назад"), tr("Up/Dn"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(CrossFactorsDefines::RLUid, CrossFactorsDefines::GroupUid,
                           tr("Отношение вправо / влево"), tr("Rt/Lf"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(CrossFactorsDefines::SFUid, CrossFactorsDefines::GroupUid,
                           tr("Отношение сагитталь / фронталь"), tr("Sg/Fr"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
}

void CrossFactors::getEventLablels()
{
    QByteArray baData;
    if (DataProvider::getChannel(probeUid(), ChannelsDefines::chanCrossResult, baData))
        m_crdata = new CrossResultData(baData);
}

void CrossFactors::fillBuffers()
{
    QByteArray baStab;
    if (DataProvider::getChannel(probeUid(), ChannelsDefines::chanStab, baStab))
    {
        Stabilogram stab(baStab);

        int kindS = 0;
        int directionS = 0;
        int counterS = 0;
        QList<QList<SignalsDefines::StabRec>> bufferUp;
        QList<QList<SignalsDefines::StabRec>> bufferDn;
        QList<QList<SignalsDefines::StabRec>> bufferLf;
        QList<QList<SignalsDefines::StabRec>> bufferRt;

        int bufCountLf = -1;
        int bufCountRt = -1;
        int bufCountUp = -1;
        int bufCountDn = -1;

        ///< Создание буфера для записи сигнала
        auto addBuffer = [&](const int direction)
        {
            if (direction == CrossDefines::dirUp)
            {
                bufferUp.append(QList<SignalsDefines::StabRec>());
                ++bufCountUp;
            }
            else
            if (direction == CrossDefines::dirDown)
            {
                bufferDn.append(QList<SignalsDefines::StabRec>());
                ++bufCountDn;
            }
            else
            if (direction == CrossDefines::dirRight)
            {
                bufferRt.append(QList<SignalsDefines::StabRec>());
                ++bufCountRt;
            }
            else
            if (direction == CrossDefines::dirLeft)
            {
                bufferLf.append(QList<SignalsDefines::StabRec>());
                ++bufCountLf;
            }
        };

        ///< Добавление значения в буфер
        auto addValueToBuffer = [&](const SignalsDefines::StabRec &val, const int direction)
        {
            if (direction == CrossDefines::dirUp)
                bufferUp[bufCountUp].append(val);
            else
            if (direction == CrossDefines::dirDown)
                bufferDn[bufCountDn].append(val);
            else
            if (direction == CrossDefines::dirRight)
                bufferRt[bufCountRt].append(val);
            else
            if (direction == CrossDefines::dirLeft)
                bufferLf[bufCountLf].append(val);
        };

        auto averageBuffer = [&] (QList<QList<SignalsDefines::StabRec>> &source, QList<SignalsDefines::StabRec> &dest)
        {
            //! Поиск минимума
            int min = INT_MAX;
            for (int i = 0; i < source.size(); ++i)
                if (source[i].size() < min)
                    min = source[i].size();

            //! Усреднение и добавление в целевой массив
            for (int i = 0; i < min; ++i)
            {
                double mx = 0;
                double my = 0;
                for (int j = 0; j < source.size(); ++j)
                {
                    mx = mx + source[j][i].x;
                    my = my + source[j][i].y;
                }
                mx = mx / source.size();
                my = my / source.size();

                SignalsDefines::StabRec rec;
                rec.x = mx;
                rec.y = my;
                dest.append(rec);
            }
        };

        //! Цикл по событиям
        for (int i = 0; i < m_crdata->eventsCount(); ++i)
        {
            int kind = 0;
            int direction = 0;
            int counter = 0;
            //! Получение события по номеру
            if (m_crdata->event(i, kind, direction, counter))
            {
                //! Начало движения
                if (kind == 1)
                {
                    kindS = kind;  //! Запоминаем параметры
                    directionS = direction;
                    counterS = counter;
                }
                else
                //! Окончание движения
                {
                    //! Новый временный буфер
                    addBuffer(direction);
                    //! Цикл по сигналу. Добавляем сигнал во временный буфер
                    for (int j = counterS; j < counter; ++j)
                    {
                        if (j >= stab.size()) break;
                        auto rec = stab.value(j);
                        addValueToBuffer(rec, direction);
                    }
                }
            }
        }
        //! Усредняем буферы
        averageBuffer(bufferUp, m_bufferUp);
        averageBuffer(bufferDn, m_bufferDn);
        averageBuffer(bufferRt, m_bufferRt);
        averageBuffer(bufferLf, m_bufferLf);
    }
}

int CrossFactors::computeDistance(QList<SignalsDefines::StabRec> &buffer, const CrossDefines::Directions direction)
{
    //! Кол-во шагов смещения цели
    int trgtLen = m_crdata->stageTime() * m_crdata->freq();
    //! Приращение смещения цели по амплитуде
    double trgtDAmpl = m_crdata->diap() / static_cast<double>(trgtLen);

    double trgtPos = 0;
    //! Цикл расчета
    for (int i = 0; i < buffer.size(); ++i)
    {
        //! Позиция цели
        trgtPos = i * trgtDAmpl;

        //! Отклонение маркера от цели
        double devMrkTrgt = 0;
        if (direction == CrossDefines::dirUp)
            devMrkTrgt = fabs(buffer[i].y - trgtPos);
        else
        if (direction == CrossDefines::dirDown)
            devMrkTrgt = fabs(buffer[i].y + trgtPos);
        else
        if (direction == CrossDefines::dirRight)
            devMrkTrgt = fabs(buffer[i].x - trgtPos);
        else
        if (direction == CrossDefines::dirLeft)
            devMrkTrgt = fabs(buffer[i].x + trgtPos);

        //! Превысили пороговое отклонение
        if (devMrkTrgt >= CrossDefines::DeviationThresholdValue)
            return static_cast<int>(trgtPos);
    }

    return m_crdata->diap();
}


