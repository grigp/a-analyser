#include "stabtesttemplate.h"

#include "metodicdefines.h"
#include "stabtestexecute.h"
#include "stabtestvisualize.h"
#include "stabtestparamsdialog.h"

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
   retval->setParams(params);
   return retval;
}

QWidget *StabTestTemplate::visualize(QWidget *parent, const QJsonObject &params)
{
    auto *retval = new StabTestVisualize(parent);
    retval->setParams(params);
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
