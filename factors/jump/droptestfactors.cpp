#include "droptestfactors.h"

#include "aanalyserapplication.h"
#include "channelsdefines.h"
#include "dataprovider.h"
#include "channelsutils.h"
#include "jumpplatedata.h"
#include "settingsprovider.h"

#include <QDebug>

DropTestFactors::DropTestFactors(const QString &testUid,
                                 const QString &probeUid,
                                 const QString &channelId,
                                 const int begin,
                                 const int end,
                                 QObject *parent)
    : ChannelMultifactor(testUid, probeUid, channelId, begin, end, parent)
{
    if (isValid())
        calculate();
}

QString DropTestFactors::uid() const
{
    return DropTestFactorsDefines::GroupUid;
}

QString DropTestFactors::name() const
{
    return nameAsConst();
}

QString DropTestFactors::nameAsConst()
{
    return QCoreApplication::tr("Показатели дроп теста");
}

bool DropTestFactors::isValid() const
{
    return isValid(testUid(), probeUid(), channelId());
}

bool DropTestFactors::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    Q_UNUSED(testUid);
    return DataProvider::channelExists(probeUid, channelId) &&
           ChannelsUtils::instance().channelType(channelId) == ChannelsDefines::ctDropTestData;
}

void DropTestFactors::calculate()
{
    m_jumps.clear();
    int jCount = 0;
    double avgHeight = 0;
    double avgContactTime = 0;
    double avgNoContactTime = 0;
    double avgReactiveForceIdx = 0;

    int massa = getPatientMassa();

    QByteArray baData;
    if (DataProvider::getChannel(probeUid(), channelId(), baData))
    {
        DropTestData data(baData);

        for (int i = 0; i < data.jumpsCount(); ++i)
        {
            SignalsDefines::DropJumpFactors djf;
            djf.height = data.jump(i).height;
            djf.timeContact = data.jump(i).timeContact;
            djf.timeNoContact = data.jump(i).timeNoContact;
            djf.fallHeight = data.jump(i).fallHeight;

            calculateAdvFactors(djf.timeContact, djf.timeNoContact, djf.fallHeight, djf.height, massa,
                                djf.power, djf.stiffness, djf.initialSpeed, djf.rsi);

            avgHeight = avgHeight + data.jump(i).height;
            avgContactTime = avgContactTime + data.jump(i).timeContact;
            avgNoContactTime = avgNoContactTime + data.jump(i).timeNoContact;
            avgReactiveForceIdx = avgReactiveForceIdx + djf.rsi;

            m_jumps.append(djf);
        }
        m_time = data.time();

        jCount = data.jumpsCount();
        if (data.jumpsCount() > 0)
        {
            avgHeight = avgHeight / jCount;
            avgContactTime = avgContactTime / jCount;
            avgNoContactTime = avgNoContactTime / jCount;
            avgReactiveForceIdx = avgReactiveForceIdx / jCount;
        }
    }

    addFactor(DropTestFactorsDefines::FullTimeUid, m_time);
    addFactor(DropTestFactorsDefines::JumpsCountUid, jCount);
    addFactor(DropTestFactorsDefines::JumpsHeightAvrUid, avgHeight);
    addFactor(DropTestFactorsDefines::JumpsContactTimeAvrUid, avgContactTime);
    addFactor(DropTestFactorsDefines::JumpsNoContactTimeAvrUid, avgNoContactTime);
    addFactor(DropTestFactorsDefines::ReactiveForceIndexUid, avgReactiveForceIdx);
}

void DropTestFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(DropTestFactorsDefines::GroupUid, nameAsConst());

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(DropTestFactorsDefines::JumpsCountUid, DropTestFactorsDefines::GroupUid,
                           QCoreApplication::tr("Количество прыжков"), QCoreApplication::tr("Кол-во"), QCoreApplication::tr(""), 0, 1, FactorsDefines::nsAbove, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(DropTestFactorsDefines::FullTimeUid, DropTestFactorsDefines::GroupUid,
                           QCoreApplication::tr("Общее время"), QCoreApplication::tr("Время"), QCoreApplication::tr("сек"), 3, 1, FactorsDefines::nsAbove, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(DropTestFactorsDefines::JumpsHeightAvrUid, DropTestFactorsDefines::GroupUid,
                           QCoreApplication::tr("Средняя высота прыжка"), QCoreApplication::tr("h"), QCoreApplication::tr("cм"), 4, 2, FactorsDefines::nsAbove, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(DropTestFactorsDefines::JumpsContactTimeAvrUid, DropTestFactorsDefines::GroupUid,
                           QCoreApplication::tr("Среднее время контактной фазы"), QCoreApplication::tr("Контакт"), QCoreApplication::tr("cек"), 4, 2, FactorsDefines::nsAbove, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(DropTestFactorsDefines::JumpsNoContactTimeAvrUid, DropTestFactorsDefines::GroupUid,
                           QCoreApplication::tr("Среднее время бесконтактной фазы"), QCoreApplication::tr("Бесконтакт"), QCoreApplication::tr("cек"), 4, 2, FactorsDefines::nsAbove, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(DropTestFactorsDefines::ReactiveForceIndexUid, DropTestFactorsDefines::GroupUid,
                           QCoreApplication::tr("Индекс реактивной силы"), QCoreApplication::tr("ИРС"), QCoreApplication::tr(""), 4, 2, FactorsDefines::nsAbove, 12);
}

int DropTestFactors::jumpsCount() const
{
    return m_jumps.size();
}

SignalsDefines::DropJumpFactors DropTestFactors::jump(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < m_jumps.size());
    return m_jumps.at(idx);
}

void DropTestFactors::calculateAdvFactors(const double timeContact, const double timeNoContact, const double fallHeight,
                                          const double height, const double massa,
                                          double &power, double &stiffness, double &initialSpeed, double &rsi)
{
    ///< Ускорение свободного падения. Зависит от местоположения, которое необходимо выбрать в настройках программы
    auto g = SettingsProvider::valueFromRegAppCopy("UserLocalize", "g", static_cast<QVariant>(9.8)).toDouble();

    power = 0;
    rsi = 0;
    if (timeContact > 0)
    {
        power = massa * g * (fallHeight * 1.226 * pow(timeNoContact, 2)) / timeContact;
        rsi = height / timeContact;
    }

    double stifZn = (pow(timeContact, 2) *((timeContact + timeNoContact) /* /n */ - timeContact / 4));
    stiffness = 0;
    if (stifZn != 0)
        stiffness = (massa * /* *n */ (timeContact + timeNoContact)) / stifZn;
    initialSpeed = sqrt(2 * g * height);  // sqrt(2*g*h)
}

int DropTestFactors::getPatientMassa()
{
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid(), ti))
    {
        DataDefines::PatientKard pk;
        if (DataProvider::getPatient(ti.patientUid, pk))
            return pk.massa;
    }
    return 0;
}
