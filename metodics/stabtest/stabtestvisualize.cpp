#include "stabtestvisualize.h"
#include "ui_stabtestvisualize.h"

#include "datadefines.h"
#include "dataprovider.h"

#include "channelsdefines.h"
#include "stabilogram.h"
#include "ballistogram.h"
#include "testresultdata.h"
#include "resultinfo.h"
#include "stabsignalstestcalculator.h"
#include "dopuskcalculator.h"
#include "statechampionstestcalculator.h"
#include "idscalculator.h"
#include "targetcalculator.h"

#include "stabsignalstestwidget.h"
#include "dopuskwidget.h"
#include "statechampionswidget.h"
#include "idswidget.h"
#include "targetwidget.h"

#include <QDebug>

StabTestVisualize::StabTestVisualize(TestCalculator* calculator, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StabTestVisualize)
  , m_calculator(calculator)
{
    ui->setupUi(this);
}

StabTestVisualize::~StabTestVisualize()
{
    delete ui;
}

void StabTestVisualize::setTest(const QString &testUid)
{
    if (m_calculator)
        m_calculator->calculate();

    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
    {
        auto params = ti.params;
        auto cnd = params["condition"].toInt();
        for (int i = 0; i < ui->wgtMain->layout()->count(); ++i)
        {
            auto *wgt = ui->wgtMain->layout()->itemAt(i)->widget();
            if (wgt)
            {
                wgt->setVisible(
                            (cnd == 0 && wgt->objectName() == "wgtSignals")
                         || (cnd == 1 && wgt->objectName() == "wgtStateChampions")
                         || (cnd == 2 && wgt->objectName() == "wgtDopusk")
                         || (cnd == 3 && wgt->objectName() == "wgtIDSAnalysis")
                         || (cnd == 4 && wgt->objectName() == "wgtTarget")
                                );
                if (cnd == 0 && wgt->objectName() == "wgtSignals")
                    static_cast<StabSignalsTestWidget*>(wgt)->
                        calculate(static_cast<StabSignalsTestCalculator*>(m_calculator), testUid);
                else
                if (cnd == 1 && wgt->objectName() == "wgtStateChampions")
                    static_cast<StateChampionsWidget*>(wgt)->
                        calculate(static_cast<StateChampionsTestCalculator*>(m_calculator), testUid);
                else
                if (cnd == 2 && wgt->objectName() == "wgtDopusk")
                    static_cast<DopuskWidget*>(wgt)->
                        calculate(static_cast<DopuskCalculator*>(m_calculator), testUid);
                else
                if (cnd == 3 && wgt->objectName() == "wgtIDSAnalysis")
                    static_cast<IDSWidget*>(wgt)->
                        calculate(static_cast<IDSCalculator*>(m_calculator), testUid);
                else
                if (cnd == 4 && wgt->objectName() == "wgtTarget")
                    static_cast<TargetWidget*>(wgt)->
                        calculate(static_cast<TargetCalculator*>(m_calculator), testUid);
            }
        }
    }

//    m_trd->openTest(testUid);
//    for (int i = 0; i < m_trd->probesCount(); ++i)
//    {
//        auto* probe = m_trd->probe(i);

//        QTextEdit *edit = new QTextEdit(ui->wgtUndef);
//        ui->wgtUndef->layout()->addWidget(edit);

//        for (int j = 0; j < probe->signalsCount(); ++j)
//        {
//            auto *signal = probe->signal(j);
//            for (int k = 0; k < signal->size(); ++k)
//            {
//                if (signal->channelId() == ChannelsDefines::chanStab)
//                {
//                     auto x = signal->value(0, k);
//                     auto y = signal->value(1, k);

//                     auto sx = QString::number(x);
//                     auto sy = QString::number(y);
//                     edit->append(sx + "     " + sy);
//                }
//                else
//                if (signal->channelId() == ChannelsDefines::chanZ)
//                {
//                    auto z = signal->value(0, k);
//                    auto sz = QString::number(z);
//                    edit->append(sz);
//                }
//            }
//            delete signal;
//        }

//        delete probe;
//    }
}

