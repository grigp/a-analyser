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
    }
    return nullptr;
}
