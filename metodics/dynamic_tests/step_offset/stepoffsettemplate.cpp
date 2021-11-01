#include "stepoffsettemplate.h"

#include "metodicdefines.h"
#include "stepoffsetparamsdialog.h"
#include "stepoffsetexecute.h"
#include "stepoffsetvisualize.h"

#include <QLayout>

StepOffsetTemplate::StepOffsetTemplate(QObject *parent)
    : MetodicTemplate (parent)
{

}

QString StepOffsetTemplate::uid()
{
    return MetodicDefines::MetUid_StepOffset;
}

QString StepOffsetTemplate::name()
{
    return MetodicDefines::MetName_StepOffset;
}

QWidget *StepOffsetTemplate::execute(QWidget *parent, const QJsonObject &params)
{
    auto *retval = new StepOffsetExecute(parent);
    parent->layout()->addWidget(retval);
    retval->setParams(params);
    return retval;
}

QWidget *StepOffsetTemplate::visualize(QWidget *parent, const QString &testUid)
{
    auto *retval = new StepOffsetVisualize(parent);
    parent->layout()->addWidget(retval);
    retval->setTest(testUid);
    return retval;
}

void StepOffsetTemplate::print(QPrinter *printer, const QString &testUid)
{
    Q_UNUSED(printer);
    Q_UNUSED(testUid);
}

bool StepOffsetTemplate::editParams(QWidget *parent, QJsonObject &params)
{
    auto dialog = new StepOffsetParamsDialog(parent);
    dialog->setParams(params);
    bool retval = false;
    if (dialog->exec() == QDialog::Accepted)
    {
        params = dialog->getParams();
        retval = true;
    }
    return retval;
}
