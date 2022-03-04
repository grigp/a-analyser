#include "bedsidescalestestertemplate.h"

#include <QLayout>

#include "dataprovider.h"
#include "metodicdefines.h"

#include "bedsidescalestesterparamsdialog.h"
#include "bedsidescalestesterexecute.h"

BedsideScalesTesterTemplate::BedsideScalesTesterTemplate(QObject *parent)
    : MetodicTemplate(parent)
{

}

QString BedsideScalesTesterTemplate::uid()
{
    return MetodicDefines::MetUid_BedsideScalesTester;
}

QString BedsideScalesTesterTemplate::name()
{
    return MetodicDefines::MetName_BedsideScalesTester;
}

QWidget *BedsideScalesTesterTemplate::execute(QWidget *parent, const QJsonObject &params)
{
    auto *retval = new BedsideScalesTesterExecute(parent);
    parent->layout()->addWidget(retval);
    retval->setParams(params);
    return retval;
}

QWidget *BedsideScalesTesterTemplate::visualize(QWidget *parent, const QString &testUid)
{
    return nullptr;
}

void BedsideScalesTesterTemplate::print(QPrinter *printer, const QString &testUid)
{

}

bool BedsideScalesTesterTemplate::editParams(QWidget *parent, QJsonObject &params)
{
    auto dialog = new BedsideScalesTesterParamsDialog(parent);
    dialog->setParams(params);
    bool retval = false;
    if (dialog->exec() == QDialog::Accepted)
    {
        params = dialog->getParams();
        retval = true;
    }
    return retval;
}
