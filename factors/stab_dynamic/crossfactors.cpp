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

QString CrossFactors::uid() const
{
    return CrossFactorsDefines::GroupUid;
}

QString CrossFactors::name() const
{
    return CrossFactorsDefines::GroupName;
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

    m_LUp = computeDistance(m_bufferUp, BaseDefines::dirUp);
    m_LDn = computeDistance(m_bufferDn, BaseDefines::dirDown);
    m_LRt = computeDistance(m_bufferRt, BaseDefines::dirRight);
    m_LLf = computeDistance(m_bufferLf, BaseDefines::dirLeft);
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
            registerGroup(CrossFactorsDefines::GroupUid, CrossFactorsDefines::GroupName);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(CrossFactorsDefines::LUpUid, CrossFactorsDefines::GroupUid,
                           tr("Отклонение вперед"), tr("Вперед"), tr("мм"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(CrossFactorsDefines::LDnUid, CrossFactorsDefines::GroupUid,
                           tr("Отклонение назад"), tr("Назад"), tr("мм"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(CrossFactorsDefines::LRtUid, CrossFactorsDefines::GroupUid,
                           tr("Отклонение вправо"), tr("Вправо"), tr("мм"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(CrossFactorsDefines::LLfUid, CrossFactorsDefines::GroupUid,
                           tr("Отклонение влево"), tr("Влево"), tr("мм"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(CrossFactorsDefines::SZoneUid, CrossFactorsDefines::GroupUid,
                           tr("Площадь зоны перермещения"), tr("Площадь"), tr("кв.мм"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(CrossFactorsDefines::UDUid, CrossFactorsDefines::GroupUid,
                           tr("Отношение вперед / назад"), tr("Вперед / Назад"), tr(""), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(CrossFactorsDefines::RLUid, CrossFactorsDefines::GroupUid,
                           tr("Отношение вправо / влево"), tr("Вправо / Влево"), tr(""), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(CrossFactorsDefines::SFUid, CrossFactorsDefines::GroupUid,
                           tr("Отношение сагитталь / фронталь"), tr("Сагит / Фронт"), tr(""), 2, 3, FactorsDefines::nsDual, 12);
}

int CrossFactors::diap() const
{
    if (m_crdata)
        return m_crdata->diap();
    else
        return 0;
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
            if (direction == BaseDefines::dirUp)
            {
                bufferUp.append(QList<SignalsDefines::StabRec>());
                ++bufCountUp;
            }
            else
            if (direction == BaseDefines::dirDown)
            {
                bufferDn.append(QList<SignalsDefines::StabRec>());
                ++bufCountDn;
            }
            else
            if (direction == BaseDefines::dirRight)
            {
                bufferRt.append(QList<SignalsDefines::StabRec>());
                ++bufCountRt;
            }
            else
            if (direction == BaseDefines::dirLeft)
            {
                bufferLf.append(QList<SignalsDefines::StabRec>());
                ++bufCountLf;
            }
        };

        ///< Добавление значения в буфер
        auto addValueToBuffer = [&](const SignalsDefines::StabRec &val, const int direction)
        {
            if (direction == BaseDefines::dirUp)
                bufferUp[bufCountUp].append(val);
            else
            if (direction == BaseDefines::dirDown)
                bufferDn[bufCountDn].append(val);
            else
            if (direction == BaseDefines::dirRight)
                bufferRt[bufCountRt].append(val);
            else
            if (direction == BaseDefines::dirLeft)
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
                    counterS = counter;  //! Запоминаем параметры
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

int CrossFactors::computeDistance(QList<SignalsDefines::StabRec> &buffer, const BaseDefines::Directions direction)
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
        if (direction == BaseDefines::dirUp)
            devMrkTrgt = fabs(buffer[i].y - trgtPos);
        else
        if (direction == BaseDefines::dirDown)
            devMrkTrgt = fabs(buffer[i].y + trgtPos);
        else
        if (direction == BaseDefines::dirRight)
            devMrkTrgt = fabs(buffer[i].x - trgtPos);
        else
        if (direction == BaseDefines::dirLeft)
            devMrkTrgt = fabs(buffer[i].x + trgtPos);

        //! Превысили пороговое отклонение
        if (devMrkTrgt >= CrossDefines::DeviationThresholdValue)
            return static_cast<int>(trgtPos);
    }

    return m_crdata->diap();
}


