#include "stepdeviationtemplate.h"

#include "metodicdefines.h"
#include "stepdeviationparamsdialog.h"
#include "stepdeviationtestexecute.h"
#include "stepdeviationvisualize.h"

#include <QLayout>
#include <QDebug>

StepDeviationTemplate::StepDeviationTemplate(QObject *parent)
    : MetodicTemplate (parent)
{

}

QString StepDeviationTemplate::uid()
{
    return MetodicDefines::MetUid_StepDeviatoin;

}

QString StepDeviationTemplate::name()
{
    return MetodicDefines::MetName_StepDeviation;
}

QWidget *StepDeviationTemplate::execute(QWidget *parent, const QJsonObject &params)
{
    auto *retval = new StepDeviationTestExecute(parent);
    parent->layout()->addWidget(retval);
    retval->setParams(params);
    return retval;
}

QWidget *StepDeviationTemplate::visualize(QWidget *parent, const QString &testUid)
{
    auto *retval = new StepDeviationVisualize(parent);
    parent->layout()->addWidget(retval);
    retval->setTest(testUid);
    return retval;
}

void StepDeviationTemplate::print(QPrinter *printer, const QString &testUid)
{
    StepDeviationVisualize::print(printer, testUid);
}

bool StepDeviationTemplate::editParams(QWidget *parent, QJsonObject &params)
{
    auto dialog = new StepDeviationParamsDialog(parent);
    dialog->setParams(params);
    bool retval = false;
    if (dialog->exec() == QDialog::Accepted)
    {
        params = dialog->getParams();
        retval = true;
    }
    return retval;
}
