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

#include "stabsignalstestwidget.h"

#include <QDebug>

StabTestVisualize::StabTestVisualize(TestCalculator* calculator, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StabTestVisualize)
  , m_calculator(calculator)
  , m_trd(new TestResultData())
{
    ui->setupUi(this);
}

StabTestVisualize::~StabTestVisualize()
{
    delete ui;
}

void StabTestVisualize::setTest(const QString &testUid)
{
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
//                         || (cnd == 1 && wgt->objectName() == "wgtStateChampions")
//                         || (cnd == 2 && wgt->objectName() == "wgtDopusk")
                                );
                if (cnd == 0 && wgt->objectName() == "wgtSignals")
                    static_cast<StabSignalsTestWidget*>(wgt)->
                        calculate(static_cast<StabSignalsTestCalculator*>(m_calculator), testUid);
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

