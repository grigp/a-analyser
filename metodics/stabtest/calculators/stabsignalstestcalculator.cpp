#include "stabsignalstestcalculator.h"

#include <QDebug>

#include "channelsdefines.h"
#include "dataprovider.h"
#include "classicfactors.h"
#include "vectorfactors.h"
#include "factorsfactory.h"
#include "aanalyserapplication.h"

StabSignalsTestCalculator::StabSignalsTestCalculator(const QString &testUid, QObject *parent)
    : StabTestCalculator(testUid, parent)
{

}

void StabSignalsTestCalculator::calculate()
{
    StabTestCalculator::calculate();
//    fillSignals();

    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid(), ti))
    {
        foreach (auto probeUid, ti.probes)
        {
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(probeUid, pi))
            {
                StabProbeFactors spf;
                spf.fctClassic = new ClassicFactors(testUid(), pi.uid, ChannelsDefines::chanStab);
                spf.fctVector = new VectorFactors(testUid(), pi.uid, ChannelsDefines::chanStab);
                m_probesFactors << spf;

                addPrimaryFactor(testUid(), ClassicFactorsDefines::MoXUid,
                                 spf.fctClassic->factorValue(ClassicFactorsDefines::MoXUid),
                                 pi.step, ChannelsDefines::chanStab, pi.name);
                addPrimaryFactor(testUid(), ClassicFactorsDefines::MoYUid,
                                 spf.fctClassic->factorValue(ClassicFactorsDefines::MoYUid),
                                 pi.step, ChannelsDefines::chanStab, pi.name);
                addPrimaryFactor(testUid(), ClassicFactorsDefines::QXUid,
                                 spf.fctClassic->factorValue(ClassicFactorsDefines::QXUid),
                                 pi.step, ChannelsDefines::chanStab, pi.name);
                addPrimaryFactor(testUid(), ClassicFactorsDefines::QYUid,
                                 spf.fctClassic->factorValue(ClassicFactorsDefines::QYUid),
                                 pi.step, ChannelsDefines::chanStab, pi.name);
                addPrimaryFactor(testUid(), ClassicFactorsDefines::RUid,
                                 spf.fctClassic->factorValue(ClassicFactorsDefines::RUid),
                                 pi.step, ChannelsDefines::chanStab, pi.name);

                addPrimaryFactor(testUid(), VectorFactorsDefines::KFRUid,
                                 spf.fctVector->factorValue(VectorFactorsDefines::KFRUid),
                                 pi.step, ChannelsDefines::chanStab, pi.name);
            }
        }
    }

}

void StabSignalsTestCalculator::fastCalculate()
{
    StabTestCalculator::fastCalculate();

}

int StabSignalsTestCalculator::factorsCount() const
{
    if (m_probesFactors.size() > 0)
        return m_probesFactors.at(0).fctClassic->size() + m_probesFactors.at(0).fctVector->size();
    return 0;
}

ClassicFactors *StabSignalsTestCalculator::classicFactors(const int probeNum) const
{
    Q_ASSERT(probeNum >= 0 && probeNum < m_probesFactors.size());
    return m_probesFactors.at(probeNum).fctClassic;
}

VectorFactors *StabSignalsTestCalculator::vectorFactors(const int probeNum) const
{
    Q_ASSERT(probeNum >= 0 && probeNum < m_probesFactors.size());
    return m_probesFactors.at(probeNum).fctVector;
}

//void StabSignalsTestCalculator::fillSignals(QModelIndex parent)
//{
//    for (int i = 0; ; ++i)
//    {
//        auto index = m_mdlTest.index(i, 0, parent);
//        if (index.isValid())
//        {
//            auto level = index.data(LevelRole).toInt();

//            //! Уровень - тест
//            if (level == LevelTest)
//                fillSignals(index);
//            else
//            //! Уровень - проба
//            if (level == LevelProbe)
//            {
//                m_probeUid = index.data(ProbeUidRole).toString();
//                fillSignals(index);
//            }
//            else
//            //! Уровень - канал
//            if (level == LevelChannel)
//            {
//                auto channelId = index.data(ChannelIdRole).toString();
//                auto channelUid = index.data(ChannelUidRole).toString();
//                QByteArray sig;
//                if (DataProvider::getChannel(channelUid, sig))
//                    m_mdlTest.itemFromIndex(index)->setData(sig, SignalRole);

//                //! Канал - стабилограмма
//                if (channelId == ChannelsDefines::chanStab)
//                {
//                    auto *fctClass = new ClassicFactors(testUid(), m_probeUid, channelId, &m_mdlTest);
//                    auto name = static_cast<AAnalyserApplication*>(QApplication::instance())->
//                            getGroupName(ClassicFactorsDefines::GroupUid);
//                    auto *item = new QStandardItem(name);
//                    item->setData(fctClass, FactorsgroupRole);
//                    item->setData(ClassicFactorsDefines::GroupUid, FactorsgroupUidRole);
//                    m_mdlTest.itemFromIndex(index)->appendColumn(item);
//                }
//            }
//        }
//    }
//}
