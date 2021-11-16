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
#include "stressstrategycalculator.h"

#include "stabsignalstestwidget.h"
#include "dopuskwidget.h"
#include "statechampionswidget.h"
#include "idswidget.h"
#include "targetwidget.h"
#include "stressstrategywidget.h"

#include <QDebug>

namespace
{
QWidget *widget {nullptr};

}

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
            widget = ui->wgtMain->layout()->itemAt(i)->widget();
            if (widget)
            {
                widget->setVisible(
                            (cnd == 0 && widget->objectName() == "wgtSignals")
                         || (cnd == 1 && widget->objectName() == "wgtStateChampions")
                         || (cnd == 2 && widget->objectName() == "wgtDopusk")
                         || (cnd == 3 && widget->objectName() == "wgtIDSAnalysis")
                         || (cnd == 4 && widget->objectName() == "wgtTarget")
                         || (cnd == 5 && widget->objectName() == "wgtStressStrategy")
                                );
                if (cnd == 0 && widget->objectName() == "wgtSignals")
                    static_cast<StabSignalsTestWidget*>(widget)->
                        calculate(static_cast<StabSignalsTestCalculator*>(m_calculator), testUid);
                else
                if (cnd == 1 && widget->objectName() == "wgtStateChampions")
                    static_cast<StateChampionsWidget*>(widget)->
                        calculate(static_cast<StateChampionsTestCalculator*>(m_calculator), testUid);
                else
                if (cnd == 2 && widget->objectName() == "wgtDopusk")
                    static_cast<DopuskWidget*>(widget)->
                        calculate(static_cast<DopuskCalculator*>(m_calculator), testUid);
                else
                if (cnd == 3 && widget->objectName() == "wgtIDSAnalysis")
                    static_cast<IDSWidget*>(widget)->
                        calculate(static_cast<IDSCalculator*>(m_calculator), testUid);
                else
                if (cnd == 4 && widget->objectName() == "wgtTarget")
                    static_cast<TargetWidget*>(widget)->
                        calculate(static_cast<TargetCalculator*>(m_calculator), testUid);
                else
                if (cnd == 5 && widget->objectName() == "wgtStressStrategy")
                    static_cast<StressStrategyWidget*>(widget)->
                        calculate(static_cast<StressStrategyCalculator*>(m_calculator), testUid);
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

void StabTestVisualize::print(QPrinter *printer, const QString &testUid)
{
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
    {
        auto params = ti.params;
        auto cnd = params["condition"].toInt();
        if (cnd == 0)
            static_cast<StabSignalsTestWidget*>(widget)->print(printer, testUid);
        else
        if (cnd == 1)
            static_cast<StateChampionsWidget*>(widget)->print(printer, testUid);
        else
        if (cnd == 2)
            static_cast<DopuskWidget*>(widget)->print(printer, testUid);
        else
        if (cnd == 3)
            static_cast<IDSWidget*>(widget)->print(printer, testUid);
        else
        if (cnd == 4)
            static_cast<TargetWidget*>(widget)->print(printer, testUid);
        else
        if (cnd == 5)
            static_cast<StressStrategyWidget*>(widget)->print(printer, testUid);
    }
}

