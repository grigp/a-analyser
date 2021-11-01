#include "stabtesttemplate.h"

#include "dataprovider.h"
#include "metodicdefines.h"
#include "stabtestexecute.h"
#include "stabtestvisualize.h"
#include "stabtestparamsdialog.h"
#include "testcalculator.h"
#include "stabsignalstestcalculator.h"
#include "dopuskcalculator.h"
#include "statechampionstestcalculator.h"
#include "idscalculator.h"
#include "targetcalculator.h"
#include "stressstrategycalculator.h"

#include <QLayout>
#include <QDebug>

StabTestTemplate::StabTestTemplate(QObject *parent)
    : MetodicTemplate(parent)
{

}

QString StabTestTemplate::uid()
{
    return MetodicDefines::MetUid_StabTest;
}

QString StabTestTemplate::name()
{
    return MetodicDefines::MetName_StabTest;
}

QWidget *StabTestTemplate::execute(QWidget *parent, const QJsonObject &params)
{
   auto *retval = new StabTestExecute(parent);
   parent->layout()->addWidget(retval);
   retval->setParams(params);
   return retval;
}

QWidget *StabTestTemplate::visualize(QWidget *parent, const QString &testUid)
{
    auto *retval = new StabTestVisualize(StabTestTemplate::calculator(testUid), parent);
    parent->layout()->addWidget(retval);
    retval->setTest(testUid);
    return retval;
}

void StabTestTemplate::print(QPrinter *printer, const QString &testUid)
{
    Q_UNUSED(printer);
    Q_UNUSED(testUid);
}

bool StabTestTemplate::editParams(QWidget *parent, QJsonObject &params)
{
    auto dialog = new StabTestParamsDialog(parent);
    dialog->setParams(params);
    bool retval = false;
    if (dialog->exec() == QDialog::Accepted)
    {
        params = dialog->getParams();
        retval = true;
    }
    return retval;
}

TestCalculator *StabTestTemplate::calculator(const QString &testUid)
{
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
    {
        auto params = ti.params;
        auto cnd = params["condition"].toInt();

        if (cnd == 0)
            return new StabSignalsTestCalculator(testUid);
        else
        if (cnd == 1)
            return new StateChampionsTestCalculator(testUid);
        else
        if (cnd == 2)
            return new DopuskCalculator(testUid);
        else
        if (cnd == 3)
            return new IDSCalculator(testUid);
        else
        if (cnd == 4)
            return new TargetCalculator(testUid);
        else
        if (cnd == 5)
            return new StressStrategyCalculator(testUid);
    }
    return nullptr;
}

QList<StabTestParams::ProbeKinds> StabTestTemplate::probeKinds()
{
    QList<StabTestParams::ProbeKinds> retval;
    for (int i = StabTestParams::pkUndefined; i < StabTestParams::pkLastValue; ++i)
        retval << static_cast<StabTestParams::ProbeKinds>(i);
    return retval;
}

QString StabTestTemplate::getProbeKindName(StabTestParams::ProbeKinds pk)
{
    static QMap<StabTestParams::ProbeKinds, QString> probeKinds {
        std::pair<StabTestParams::ProbeKinds, QString> (StabTestParams::pkUndefined, tr("Не задана"))
      , std::pair<StabTestParams::ProbeKinds, QString> (StabTestParams::pkBackground, tr("Фоновая"))
      , std::pair<StabTestParams::ProbeKinds, QString> (StabTestParams::pkCloseEyes, tr("Закрытые глаза"))
      , std::pair<StabTestParams::ProbeKinds, QString> (StabTestParams::pkTarget, tr("Мишень"))
      , std::pair<StabTestParams::ProbeKinds, QString> (StabTestParams::pkFiveZones, tr("Выделенная зона"))
      , std::pair<StabTestParams::ProbeKinds, QString> (StabTestParams::pkRugStand, tr("Стойка на коврике"))
      , std::pair<StabTestParams::ProbeKinds, QString> (StabTestParams::pkMandibular, tr("Мандибулярная (сжатые челюсти)"))
      , std::pair<StabTestParams::ProbeKinds, QString> (StabTestParams::pkHeadToRight, tr("Голова повернута направо"))
      , std::pair<StabTestParams::ProbeKinds, QString> (StabTestParams::pkHeadToLeft, tr("Голова повернута налево"))
      , std::pair<StabTestParams::ProbeKinds, QString> (StabTestParams::pkHeadTiltRight, tr("Голова наклонена вправо"))
      , std::pair<StabTestParams::ProbeKinds, QString> (StabTestParams::pkHeadTiltLeft, tr("Голова наклонена влево"))
      , std::pair<StabTestParams::ProbeKinds, QString> (StabTestParams::pkHeadTiltForward, tr("Голова наклонена вперед"))
      , std::pair<StabTestParams::ProbeKinds, QString> (StabTestParams::pkHeadTiltBackward, tr("Голова наклонена назад"))
      , std::pair<StabTestParams::ProbeKinds, QString> (StabTestParams::pkLinesToRight, tr("Движение полос вправо"))
      , std::pair<StabTestParams::ProbeKinds, QString> (StabTestParams::pkLinesToLeft, tr("Движение полос влево"))
      , std::pair<StabTestParams::ProbeKinds, QString> (StabTestParams::pkLinesUp, tr("Движение полос вверх"))
      , std::pair<StabTestParams::ProbeKinds, QString> (StabTestParams::pkLinesDown, tr("Движение полос вниз"))
//      , std::pair<StabTestParams::ProbeKinds, QString> (StabTestParams::pkDynamic, tr("Динамическая"))
      , std::pair<StabTestParams::ProbeKinds, QString> (StabTestParams::pkLastValue, tr("Последнее значение"))
    };

    if (probeKinds.contains(pk))
        return probeKinds.value(pk);
    return tr("Не задана");
}
