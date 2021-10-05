#include "boxerdodgingmultifactor.h"

#include "aanalyserapplication.h"
#include "dataprovider.h"
#include "channelsdefines.h"
#include "boxerdodgingdefines.h"
#include "boxerdodgingresultdata.h"
#include "stabilogram.h"

#include <QDebug>

BoxerDodgingMultifactor::BoxerDodgingMultifactor(const QString &testUid,
                                                 const QString &probeUid,
                                                 QObject *parent)
    : ProbeMultifactor (testUid, probeUid, parent)
{
    if (isValid())
        calculate();
}

bool BoxerDodgingMultifactor::isValid() const
{
    return isValid(testUid(), probeUid());
}

bool BoxerDodgingMultifactor::isValid(const QString &testUid, const QString &probeUid)
{
    Q_UNUSED(testUid);
    auto de = DataProvider::channelExists(probeUid, ChannelsDefines::chanStab);
    auto se = DataProvider::channelExists(probeUid, ChannelsDefines::chanBoxerDodgingResult);
    return de && se;
}

void BoxerDodgingMultifactor::calculate()
{

    //! Получение стабилограммы
    QByteArray baStab;
    if (DataProvider::getChannel(probeUid(), ChannelsDefines::chanStab, baStab))
    {
        Stabilogram stab(baStab);

        getEventLablels();
        assignSections(&stab);
        calculateFactors(&stab);
    }

    addFactor(BoxerDodgingFactorsDefines::AverageLatUid, m_fctAverage.latent);
    addFactor(BoxerDodgingFactorsDefines::AverageTimeUid, m_fctAverage.time);
    addFactor(BoxerDodgingFactorsDefines::AverageAmplUid, m_fctAverage.ampl);
    addFactor(BoxerDodgingFactorsDefines::AllErrorsUid, m_fctAverage.errors);

    addFactor(BoxerDodgingFactorsDefines::LeftLatUid, m_fctLeft.latent);
    addFactor(BoxerDodgingFactorsDefines::LeftTimeUid, m_fctLeft.time);
    addFactor(BoxerDodgingFactorsDefines::LeftAmplUid, m_fctLeft.ampl);
    addFactor(BoxerDodgingFactorsDefines::LeftErrorsUid, m_fctLeft.errors);

    addFactor(BoxerDodgingFactorsDefines::RightLatUid, m_fctRight.latent);
    addFactor(BoxerDodgingFactorsDefines::RightTimeUid, m_fctRight.time);
    addFactor(BoxerDodgingFactorsDefines::RightAmplUid, m_fctRight.ampl);
    addFactor(BoxerDodgingFactorsDefines::RightErrorsUid, m_fctRight.errors);

    addFactor(BoxerDodgingFactorsDefines::AheadLatUid, m_fctAhead.latent);
    addFactor(BoxerDodgingFactorsDefines::AheadTimeUid, m_fctAhead.time);
    addFactor(BoxerDodgingFactorsDefines::AheadAmplUid, m_fctAhead.ampl);
    addFactor(BoxerDodgingFactorsDefines::AheadErrorsUid, m_fctAhead.errors);

    addFactor(BoxerDodgingFactorsDefines::BackLatUid, m_fctBack.latent);
    addFactor(BoxerDodgingFactorsDefines::BackTimeUid, m_fctBack.time);
    addFactor(BoxerDodgingFactorsDefines::BackAmplUid, m_fctBack.ampl);
    addFactor(BoxerDodgingFactorsDefines::BackErrorsUid, m_fctBack.errors);
}

void BoxerDodgingMultifactor::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(BoxerDodgingFactorsDefines::GroupUid, tr("Показатели тренажера с уклонением"));

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::AverageLatUid, BoxerDodgingFactorsDefines::GroupUid,
                           tr("Средний латентный период"), tr("LatMid"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::AverageTimeUid, BoxerDodgingFactorsDefines::GroupUid,
                           tr("Среднее время реакции"), tr("TimeMid"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::AverageAmplUid, BoxerDodgingFactorsDefines::GroupUid,
                           tr("Средняя амплитуда"), tr("AmplMid"), tr("мм"), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::AllErrorsUid, BoxerDodgingFactorsDefines::GroupUid,
                           tr("Количество ошибок"), tr("Errors"), tr(""), 0, 3, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::LeftLatUid, BoxerDodgingFactorsDefines::GroupUid,
                           tr("Латентный период влево"), tr("LatLeft"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::LeftTimeUid, BoxerDodgingFactorsDefines::GroupUid,
                           tr("Время реакции влево"), tr("TimeLeft"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::LeftAmplUid, BoxerDodgingFactorsDefines::GroupUid,
                           tr("Амплитуда влево"), tr("AmplLeft"), tr("мм"), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::LeftErrorsUid, BoxerDodgingFactorsDefines::GroupUid,
                           tr("Количество ошибок влево"), tr("ErrorsLeft"), tr(""), 0, 3, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::RightLatUid, BoxerDodgingFactorsDefines::GroupUid,
                           tr("Латентный период вправо"), tr("LatRight"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::RightTimeUid, BoxerDodgingFactorsDefines::GroupUid,
                           tr("Время реакции вправо"), tr("TimeRight"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::RightAmplUid, BoxerDodgingFactorsDefines::GroupUid,
                           tr("Амплитуда вправо"), tr("AmplRight"), tr("мм"), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::RightErrorsUid, BoxerDodgingFactorsDefines::GroupUid,
                           tr("Количество ошибок вправо"), tr("ErrorsRight"), tr(""), 0, 3, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::AheadLatUid, BoxerDodgingFactorsDefines::GroupUid,
                           tr("Латентный период вперед"), tr("LatAhead"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::AheadTimeUid, BoxerDodgingFactorsDefines::GroupUid,
                           tr("Время реакции вперед"), tr("TimeAhead"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::AheadAmplUid, BoxerDodgingFactorsDefines::GroupUid,
                           tr("Амплитуда вперед"), tr("AmplAhead"), tr("мм"), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::AheadErrorsUid, BoxerDodgingFactorsDefines::GroupUid,
                           tr("Количество ошибок вперед"), tr("ErrorsAhead"), tr(""), 0, 3, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::BackLatUid, BoxerDodgingFactorsDefines::GroupUid,
                           tr("Латентный период назад"), tr("LatBack"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::BackTimeUid, BoxerDodgingFactorsDefines::GroupUid,
                           tr("Время реакции назад"), tr("TimeBack"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::BackAmplUid, BoxerDodgingFactorsDefines::GroupUid,
                           tr("Амплитуда назад"), tr("AmplBack"), tr("мм"), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::BackErrorsUid, BoxerDodgingFactorsDefines::GroupUid,
                           tr("Количество ошибок назад"), tr("ErrorsBack"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
}

int BoxerDodgingMultifactor::deviationThreshold() const
{
    if (m_resData)
        return m_resData->deviationThreshold();
    return 0;
}

int BoxerDodgingMultifactor::freq() const
{
    if (m_resData)
        return m_resData->freq();
    return 50;
}

int BoxerDodgingMultifactor::diap() const
{
    if (m_resData)
        return m_resData->diap();
    return 0;
}

void BoxerDodgingMultifactor::getEventLablels()
{
    QByteArray baData;
    if (DataProvider::getChannel(probeUid(), ChannelsDefines::chanBoxerDodgingResult, baData))
        m_resData = new BoxerDodgingResultData(baData);
}

void BoxerDodgingMultifactor::assignSections(Stabilogram* stab)
{
    int dirCode {0};
    int begin {0};
    int end {0};
    bool isBegin {false};

    auto addSection = [&]()
    {
        if (dirCode == BoxerDodgingDefines::bdsLeftDodging)
            m_secLeft << Section(begin, end);
        else
        if (dirCode == BoxerDodgingDefines::bdsRightDodging)
            m_secRight << Section(begin, end);
        else
        if (dirCode == BoxerDodgingDefines::bdsAheadBend)
            m_secAhead << Section(begin, end);
        else
        if (dirCode == BoxerDodgingDefines::bdsBackBend)
            m_secBack << Section(begin, end);
    };


    for (int i = 0; i < m_resData->stagesCount(); ++i)
    {
        int code {0};
        int counter {0};
        m_resData->stage(i, code, counter);
        if (code != BoxerDodgingDefines::bdsBase)
        {
            dirCode = code;
            begin = counter;
            isBegin = true;
        }
        else
        {
            end = counter;
            addSection();
            isBegin = false;
        }
    }

    //! Последняя секция в конце, не заканчивается меткой базовой стойки.
    //! Надо отработать до конца сигнала
    if (isBegin)
    {
        end = stab->size();
        addSection();
    }
}

void BoxerDodgingMultifactor::calculateFactors(Stabilogram* stab)
{
    //! Количества пишем сразу
    m_fctLeft.count = m_secLeft.size();
    m_fctRight.count = m_secRight.size();
    m_fctAhead.count = m_secAhead.size();
    m_fctBack.count = m_secBack.size();

    //! Расчет по направлениям
        calculateFactorsForDirection(m_secLeft, m_fctLeft, stab);
        calculateFactorsForDirection(m_secRight, m_fctRight, stab);
        calculateFactorsForDirection(m_secAhead, m_fctAhead, stab);
        calculateFactorsForDirection(m_secBack, m_fctBack, stab);
}

void BoxerDodgingMultifactor::calculateFactorsForDirection(const QList<BoxerDodgingMultifactor::Section> sections,
                                                           BoxerDodgingMultifactor::FactorsDirection &factors,
                                                           Stabilogram* stab)
{
    //! Инициализация
    factors.latent = 0;
    factors.time = 0;
    factors.ampl = 0;
    factors.errors = 0;

    //! По участкам направления
    for (int i = 0; i < sections.size(); ++i)
    {
        //! По сигналу на участке
        for (int j = sections.at(i).begin; j < sections.at(i).end; ++j)
        {

        }
    }
}

