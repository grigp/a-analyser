#include "stabsignalstestcalculator.h"

#include "channelsdefines.h"
#include "dataprovider.h"
#include "classicfactors.h"
#include "factorsfactory.h"
#include "aanalyserapplication.h"

StabSignalsTestCalculator::StabSignalsTestCalculator(const QString &testUid, QObject *parent)
    : StabTestCalculator(testUid, parent)
{

}

void StabSignalsTestCalculator::calculate()
{
    StabTestCalculator::calculate();
    fillSignals();
}

void StabSignalsTestCalculator::fastCalculate()
{
    StabTestCalculator::fastCalculate();

}

void StabSignalsTestCalculator::fillSignals(QModelIndex parent)
{
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
}
