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
#include "reportelements.h"

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
    QPainter *painter = new QPainter(printer);
    QRect paper = printer->pageRect();

    qDebug() << "1";
    painter->begin(printer);
    qDebug() << "2";

    painter->setPen(Qt::black);
    painter->setBrush(QBrush(Qt::red));
    painter->drawRect(10, 10, 410, 410);
    painter->setFont(QFont("Sans",64,0,0));
    painter->drawText(QRect(0,0,3000,800), Qt::AlignLeft | Qt::AlignTop, "page1");
    qDebug() << "3";

    //! Заголовок
//    QRect rectHeader(paper.x() + paper.width() / 20, paper.y() + paper.height() / 30, paper.width() / 20 * 18, paper.height() / 30 * 3);
//    ReportElements::drawHeader(painter, testUid, rectHeader);

    if (printer->orientation() == QPrinter::Portrait)
    {
        //! Диаграмма Cross. Копируется из виджета
        qDebug() << "3";
//        ReportElements::drawWidget(painter, ui->wgtOpenEyesNorm,
//                                   static_cast<int>(paper.width() * 0.8), static_cast<int>(paper.height() * 0.8),
//                                   paper.x() + paper.width()/10, paper.y() + paper.height()/7);
        qDebug() << "4";

//        //! Таблица показателей. Берется модель таблицы из визуализатора
//        QRect rectTable(paper.x() + paper.width() / 10,
//                        paper.y() + paper.height() / 3 * 2,
//                        paper.width() / 10 * 8,
//                        paper.height() / 4);
//        ReportElements::drawTable(painter, mdlFactors, rectTable,
//                                  QList<int>() << 3 << 1, ReportElements::Table::tvsStretched,
//                                  12, -1, QFont::Bold);
    }
    else
    if (printer->orientation() == QPrinter::Landscape)
    {
        //! Диаграмма Cross. Копируется из виджета
//        ReportElements::drawWidget(painter, ui->frOpenEyes,
//                                   static_cast<int>(paper.width() * 0.6), static_cast<int>(paper.height() * 0.6),
//                                   paper.x() + paper.width()/20, paper.y() + paper.height()/4);

//        //! Таблица показателей. Берется модель таблицы из визуализатора
//        QRect rectTable(paper.x() + paper.width() / 7 * 4,
//                        static_cast<int>(paper.y() + paper.height() / 3.5),
//                        paper.width() / 7 * 3,
//                        paper.height() / 2);
//        ReportElements::drawTable(painter, mdlFactors, rectTable,
//                                  QList<int>() << 3 << 1, ReportElements::Table::tvsStretched,
//                                  12, -1, QFont::Bold);
    }

    //! Нижний колонтитул
    QRect rectFooter(paper.x() + paper.width() / 20,
                     paper.y() + paper.height() - static_cast<int>(paper.height() / 30 * 1.5),
                     paper.width() / 20 * 18,
                     static_cast<int>(paper.height() / 30 * 1.5));
    ReportElements::drawFooter(painter, testUid, rectFooter);

    painter->end();

//    DataDefines::TestInfo ti;
//    if (DataProvider::getTestInfo(testUid, ti))
//    {
//        auto params = ti.params;
//        auto cnd = params["condition"].toInt();
//        if (cnd == 0)
//            static_cast<StabSignalsTestWidget*>(widget)->print(printer, testUid);
//        else
//        if (cnd == 1)
//            static_cast<StateChampionsWidget*>(widget)->print(printer, testUid);
//        else
//        if (cnd == 2)
//            static_cast<DopuskWidget*>(widget)->print(printer, testUid);
//        else
//        if (cnd == 3)
//            static_cast<IDSWidget*>(widget)->print(printer, testUid);
//        else
//        if (cnd == 4)
//            static_cast<TargetWidget*>(widget)->print(printer, testUid);
//        else
//        if (cnd == 5)
//            static_cast<StressStrategyWidget*>(widget)->print(printer, testUid);
//    }
}

