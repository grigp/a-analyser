#include "trentakeputtemplate.h"

#include "metodicdefines.h"
#include "trentakeputexecute.h"
#include "trentakeputvisualize.h"
#include "trentakeputparamsdialog.h"

#include <QLayout>

TrenTakePutTemplate::TrenTakePutTemplate(QObject *parent)
    : MetodicTemplate(parent)
{

}

QString TrenTakePutTemplate::uid()
{
    return MetodicDefines::MetUid_TrenTakePut;
}

QString TrenTakePutTemplate::name()
{
    return MetodicDefines::MetName_TrenTakePut;
}

QWidget *TrenTakePutTemplate::execute(QWidget *parent, const QJsonObject &params)
{
    auto *retval = new TrenTakePutExecute(parent);
    retval->setParams(params);
    parent->layout()->addWidget(retval);
    return retval;
}

QWidget *TrenTakePutTemplate::visualize(QWidget *parent, const QString &testUid)
{
    auto *retval = new TrenTakePutVisualize(parent);
    parent->layout()->addWidget(retval);
    retval->setTest(testUid);
    return retval;
}

bool TrenTakePutTemplate::editParams(QWidget *parent, QJsonObject &params)
{
    auto dialog = new TrenTakePutParamsDialog(parent);
    dialog->setParams(params);
    bool retval = false;
    if (dialog->exec() == QDialog::Accepted)
    {
        params = dialog->getParams();
        retval = true;
    }
    return retval;
}