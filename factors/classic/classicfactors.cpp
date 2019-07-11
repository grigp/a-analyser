#include "classicfactors.h"

#include "channelsdefines.h"
#include "channelsutils.h"
#include "dataprovider.h"
#include "stabilogram.h"
#include "factorsfactory.h"
#include "aanalyserapplication.h"

#include <QByteArray>
#include <QDebug>

ClassicFactors::ClassicFactors(const QString &testUid,
                               const QString &probeUid,
                               const QString &channelId,
                               QObject *parent)
    : ChannelMultifactor(testUid, probeUid, channelId, parent)
{
    if (isValid())
        calculate();
}

bool ClassicFactors::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    Q_UNUSED(testUid);
    return DataProvider::channelExists(probeUid, channelId) &&
           ChannelsUtils::instance().channelType(channelId) == ChannelsDefines::ctStabilogram;
}

bool ClassicFactors::isValid() const
{
    return isValid(testUid(), probeUid(), channelId());
//    return DataProvider::channelExists(probeUid(), channelId()) &&
//           ChannelsUtils::instance().channelType(channelId()) == ChannelsDefines::ctStabilogram;
}

void ClassicFactors::calculate()
{
    QByteArray baStab;
    if (DataProvider::getChannel(probeUid(), channelId(), baStab))
    {
        Stabilogram stab(baStab);

        for (int i = 0; i < stab.size(); ++i)
        {
            auto rec = stab.value(i);
            m_mx = m_mx + rec.x;
            m_my = m_my + rec.y;
        }
        m_mx = m_mx / stab.size();
        m_my = m_my / stab.size();

        for (int i = 0; i < stab.size(); ++i)
        {
            auto rec = stab.value(i);
            m_qx = m_qx + pow(abs(rec.x - m_mx), 2) / (stab.size() - 1);
            m_qy = m_qy + pow(abs(rec.y - m_my), 2) / (stab.size() - 1);
            m_r = m_r + sqrt(pow(rec.x - m_mx, 2) + pow(rec.y - m_my, 2)) / stab.size();
        }
        m_qx = sqrt(m_qx);
        m_qy = sqrt(m_qy);
    }

    addFactor(ClassicFactorsDefines::MoXUid, m_mx);
    addFactor(ClassicFactorsDefines::MoYUid, m_my);
    addFactor(ClassicFactorsDefines::QXUid, m_qx);
    addFactor(ClassicFactorsDefines::QYUid, m_qy);
    addFactor(ClassicFactorsDefines::RUid, m_r);
}

void ClassicFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(ClassicFactorsDefines::GroupUid, tr("Классические показатели"));

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(ClassicFactorsDefines::MoXUid, ClassicFactorsDefines::GroupUid,  tr("Смещение по фронтали"), tr("MoX"), tr("мм"), "");
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(ClassicFactorsDefines::MoYUid, ClassicFactorsDefines::GroupUid,  tr("Смещение по сагиттали"), tr("MoY"), tr("мм"), "");
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(ClassicFactorsDefines::QXUid, ClassicFactorsDefines::GroupUid,  tr("Разброс по фронтали"), tr("QX"), tr("мм"), "");
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(ClassicFactorsDefines::QYUid, ClassicFactorsDefines::GroupUid,  tr("Разброс по сагиттали"), tr("QY"), tr("мм"), "");
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(ClassicFactorsDefines::RUid, ClassicFactorsDefines::GroupUid,  tr("Средний радиус"), tr("R"), tr("мм"), "");
}
