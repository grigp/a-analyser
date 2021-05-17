#include "crosstemplate.h"

#include "metodicdefines.h"
#include "crossparamsdialog.h"
#include "crossexecute.h"
#include "crossvisualize.h"

#include <QLayout>

CrossTemplate::CrossTemplate(QObject *parent)
    : MetodicTemplate (parent)
{

}

QString CrossTemplate::uid()
{
    return MetodicDefines::MetUid_Cross;
}

QString CrossTemplate::name()
{
    return MetodicDefines::MetName_Cross;
}

QWidget *CrossTemplate::execute(QWidget *parent, const QJsonObject &params)
{
    auto *retval = new CrossExecute(parent);
    parent->layout()->addWidget(retval);
    retval->setParams(params);
    return retval;
}

QWidget *CrossTemplate::visualize(QWidget *parent, const QString &testUid)
{
    auto *retval = new CrossVisualize(parent);
    parent->layout()->addWidget(retval);
    retval->setTest(testUid);
    return retval;
}

bool CrossTemplate::editParams(QWidget *parent, QJsonObject &params)
{
    auto dialog = new CrossParamsDialog(parent);
    dialog->setParams(params);
    bool retval = false;
    if (dialog->exec() == QDialog::Accepted)
    {
        params = dialog->getParams();
        retval = true;
    }
    return retval;
}
