#include "octaedrontemplate.h"

#include <QLayout>

#include "metodicdefines.h"
#include "octaedronparamsdialog.h"
#include "octaedronexecute.h"
#include "octaedronvisualize.h"

OctaedronTemplate::OctaedronTemplate(QObject *parent)
    : MetodicTemplate (parent)
{

}

QString OctaedronTemplate::uid()
{
    return MetodicDefines::MetUid_Octaedron;
}

QString OctaedronTemplate::name()
{
    return MetodicDefines::MetName_Octaedron;
}

QWidget *OctaedronTemplate::execute(QWidget *parent, const QJsonObject &params)
{
    auto *retval = new OctaedronExecute(parent);
    parent->layout()->addWidget(retval);
    retval->setParams(params);
    return retval;
}

QWidget *OctaedronTemplate::visualize(QWidget *parent, const QString &testUid)
{
    auto *retval = new OctaedronVisualize(parent);
    parent->layout()->addWidget(retval);
    retval->setTest(testUid);
    return retval;
}

void OctaedronTemplate::print(QPrinter *printer, const QString &testUid)
{
    Q_UNUSED(printer);
    Q_UNUSED(testUid);
}

bool OctaedronTemplate::editParams(QWidget *parent, QJsonObject &params)
{
    auto dialog = new OctaedronParamsDialog(parent);
    dialog->setParams(params);
    bool retval = false;
    if (dialog->exec() == QDialog::Accepted)
    {
        params = dialog->getParams();
        retval = true;
    }
    return retval;
}
