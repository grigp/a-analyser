#include "boxerdodgingmultifactor.h"

#include "baseutils.h"
#include "aanalyserapplication.h"
#include "dataprovider.h"
#include "channelsdefines.h"
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

QString BoxerDodgingMultifactor::uid() const
{
    return BoxerDodgingFactorsDefines::GroupUid;
}

QString BoxerDodgingMultifactor::name() const
{
    return nameAsConst();
}

QString BoxerDodgingMultifactor::nameAsConst()
{
    return QCoreApplication::tr("Показатели тренажера с уклонением");
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
            registerGroup(BoxerDodgingFactorsDefines::GroupUid, nameAsConst());

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::AverageLatUid, BoxerDodgingFactorsDefines::GroupUid,
                           QCoreApplication::tr("Средний латентный период"), QCoreApplication::tr("LatMid"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::AverageTimeUid, BoxerDodgingFactorsDefines::GroupUid,
                           QCoreApplication::tr("Среднее время реакции"), QCoreApplication::tr("TimeMid"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::AverageAmplUid, BoxerDodgingFactorsDefines::GroupUid,
                           QCoreApplication::tr("Средняя амплитуда"), QCoreApplication::tr("AmplMid"), QCoreApplication::tr("мм"), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::AllErrorsUid, BoxerDodgingFactorsDefines::GroupUid,
                           QCoreApplication::tr("Количество ошибок"), QCoreApplication::tr("Errors"), QCoreApplication::tr(""), 0, 3, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::LeftLatUid, BoxerDodgingFactorsDefines::GroupUid,
                           QCoreApplication::tr("Латентный период влево"), QCoreApplication::tr("LatLeft"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::LeftTimeUid, BoxerDodgingFactorsDefines::GroupUid,
                           QCoreApplication::tr("Время реакции влево"), QCoreApplication::tr("TimeLeft"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::LeftAmplUid, BoxerDodgingFactorsDefines::GroupUid,
                           QCoreApplication::tr("Амплитуда влево"), QCoreApplication::tr("AmplLeft"), QCoreApplication::tr("мм"), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::LeftErrorsUid, BoxerDodgingFactorsDefines::GroupUid,
                           QCoreApplication::tr("Количество ошибок влево"), QCoreApplication::tr("ErrorsLeft"), QCoreApplication::tr(""), 0, 3, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::RightLatUid, BoxerDodgingFactorsDefines::GroupUid,
                           QCoreApplication::tr("Латентный период вправо"), QCoreApplication::tr("LatRight"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::RightTimeUid, BoxerDodgingFactorsDefines::GroupUid,
                           QCoreApplication::tr("Время реакции вправо"), QCoreApplication::tr("TimeRight"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::RightAmplUid, BoxerDodgingFactorsDefines::GroupUid,
                           QCoreApplication::tr("Амплитуда вправо"), QCoreApplication::tr("AmplRight"), QCoreApplication::tr("мм"), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::RightErrorsUid, BoxerDodgingFactorsDefines::GroupUid,
                           QCoreApplication::tr("Количество ошибок вправо"), QCoreApplication::tr("ErrorsRight"), QCoreApplication::tr(""), 0, 3, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::AheadLatUid, BoxerDodgingFactorsDefines::GroupUid,
                           QCoreApplication::tr("Латентный период вперед"), QCoreApplication::tr("LatAhead"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::AheadTimeUid, BoxerDodgingFactorsDefines::GroupUid,
                           QCoreApplication::tr("Время реакции вперед"), QCoreApplication::tr("TimeAhead"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::AheadAmplUid, BoxerDodgingFactorsDefines::GroupUid,
                           QCoreApplication::tr("Амплитуда вперед"), QCoreApplication::tr("AmplAhead"), QCoreApplication::tr("мм"), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::AheadErrorsUid, BoxerDodgingFactorsDefines::GroupUid,
                           QCoreApplication::tr("Количество ошибок вперед"), QCoreApplication::tr("ErrorsAhead"), QCoreApplication::tr(""), 0, 3, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::BackLatUid, BoxerDodgingFactorsDefines::GroupUid,
                           QCoreApplication::tr("Латентный период назад"), QCoreApplication::tr("LatBack"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::BackTimeUid, BoxerDodgingFactorsDefines::GroupUid,
                           QCoreApplication::tr("Время реакции назад"), QCoreApplication::tr("TimeBack"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::BackAmplUid, BoxerDodgingFactorsDefines::GroupUid,
                           QCoreApplication::tr("Амплитуда назад"), QCoreApplication::tr("AmplBack"), QCoreApplication::tr("мм"), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(BoxerDodgingFactorsDefines::BackErrorsUid, BoxerDodgingFactorsDefines::GroupUid,
                           QCoreApplication::tr("Количество ошибок назад"), QCoreApplication::tr("ErrorsBack"), QCoreApplication::tr(""), 0, 3, FactorsDefines::nsDual, 12);
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
            m_secLeft << BaseDefines::Section(begin, end);
        else
        if (dirCode == BoxerDodgingDefines::bdsRightDodging)
            m_secRight << BaseDefines::Section(begin, end);
        else
        if (dirCode == BoxerDodgingDefines::bdsAheadBend)
            m_secAhead << BaseDefines::Section(begin, end);
        else
        if (dirCode == BoxerDodgingDefines::bdsBackBend)
            m_secBack << BaseDefines::Section(begin, end);
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
    m_fctAverage.count = m_secLeft.size() + m_secRight.size() + m_secAhead.size() + m_secBack.size();

    //! Расчет по направлениям
    calculateFactorsForDirection(BoxerDodgingDefines::bdsLeftDodging, m_secLeft, m_fctLeft, stab);
    calculateFactorsForDirection(BoxerDodgingDefines::bdsRightDodging, m_secRight, m_fctRight, stab);
    calculateFactorsForDirection(BoxerDodgingDefines::bdsAheadBend, m_secAhead, m_fctAhead, stab);
    calculateFactorsForDirection(BoxerDodgingDefines::bdsBackBend, m_secBack, m_fctBack, stab);

    //! Усредненнве по направлениям
    averaging();
}

void BoxerDodgingMultifactor::averaging()
{
    m_fctAverage.errors = m_fctLeft.errors + m_fctRight.errors + m_fctAhead.errors + m_fctBack.errors;
    m_fctAverage.latent = 0;
    m_fctAverage.time = 0;
    m_fctAverage.ampl = 0;

    int n = 0;

    auto addValues = [&](FactorsDirection& factors)
    {
        if (factors.count > 0)
        {
            m_fctAverage.latent += factors.latent;
            m_fctAverage.time += factors.time;
            m_fctAverage.ampl += factors.ampl;
            ++n;
        }
    };

    addValues(m_fctLeft);
    addValues(m_fctRight);
    addValues(m_fctAhead);
    addValues(m_fctBack);

    if (n > 0)
    {
        m_fctAverage.latent /= n;
        m_fctAverage.time /= n;
        m_fctAverage.ampl /= n;
    }
}

void BoxerDodgingMultifactor::getDataBuffer(Stabilogram *stab, QVector<double> *buf,
                                            const BoxerDodgingDefines::Stages code,
                                            const int begin, const int end)
{
    //! Проверка границ участка
    int b = begin;
    if (b > stab->size()) b = stab->size();
    int e = end;
    if (e > stab->size()) e = stab->size();

    //! Буфер должен быть пуст
    buf->clear();
    //! Заполнение
    for (int i = b; i < e; ++i)
    {
        if (code == BoxerDodgingDefines::bdsLeftDodging)
            buf->append(-stab->value(0, i));
        else
        if (code == BoxerDodgingDefines::bdsRightDodging)
            buf->append(stab->value(0, i));
        else
        if (code == BoxerDodgingDefines::bdsAheadBend)
            buf->append(stab->value(1, i));
        else
        if (code == BoxerDodgingDefines::bdsBackBend)
            buf->append(-stab->value(1, i));
    }
}

void BoxerDodgingMultifactor::calculateFactorsForDirection(const BoxerDodgingDefines::Stages code,
                                                           const QList<BaseDefines::Section> sections,
                                                           BoxerDodgingMultifactor::FactorsDirection &factors,
                                                           Stabilogram* stab)
{
    //! Инициализация
    factors.latent = 0;
    factors.time = 0;
    factors.ampl = 0;
    factors.errors = 0;

    int ln = 0;  //! Счетчик попыток для латентного периода
    //! По участкам направления
    for (int i = 0; i < sections.size(); ++i)
    {
        //! Выделение участка в отдельный буфер с диапзоном сигнала 0 - n
        QVector<double> buffer;
        getDataBuffer(stab, &buffer, code, sections.at(i).begin, sections.at(i).end);

        //! Латентный период
        BaseUtils::MidAndStandardDeviation msd;
        //! Среднее за 0,5 сек от начала
        for (int j = 0; j < buffer.size() && j < 0.5 * m_resData->freq(); ++j)
            msd.add(buffer[j]);
        double m = 0;
        double q = 0;
        msd.calculate(m, q);
        //! Расчет латентного периода
        int l = 0;
        for (int j = m_resData->freq() / 4; j < buffer.size() && fabs(buffer[j] - m) < 1 * q; ++j)
            l = j;
//        factors.latent += (static_cast<double>(l) / static_cast<double>(m_resData->freq()));
//        ++ln;

        //! Время реакции
        int t = 0;
        for (int j = 0; j < buffer.size(); ++j)
            if (buffer[j] > m_resData->deviationThreshold())
            {
                t = j;
                break;
            }
        factors.time += (static_cast<double>(t) / static_cast<double>(m_resData->freq()));
        //! Ошибки
        if (t == 0)
            ++factors.errors;
        else
        {
            factors.latent += (static_cast<double>(l) / static_cast<double>(m_resData->freq()));
            ++ln;
        }

        //! Амплитуда
        double a = 0;
        for (int j = 0; j < buffer.size(); ++j)
        {
            if (buffer[j] > a && buffer[j] > m_resData->deviationThreshold())
                a = buffer[j];
        }
        factors.ampl += a;
    }

    //! Усреднение
    if (ln > 0)
        factors.latent /= static_cast<double>(ln);
    if (sections.size() > 0)
    {
//        factors.latent /= static_cast<double>(sections.size());
        factors.time /= static_cast<double>(sections.size());
        factors.ampl /= static_cast<double>(sections.size());
    }
}

