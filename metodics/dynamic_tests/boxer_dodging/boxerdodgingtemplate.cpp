#include "boxerdodgingtemplate.h"

#include <QLayout>

#include "metodicdefines.h"
#include "boxerdodgingparamsdialog.h"
#include "boxerdodgingexecute.h"

BoxerDodgingTemplate::BoxerDodgingTemplate(QObject *parent)
    : MetodicTemplate(parent)
{

}

QString BoxerDodgingTemplate::uid()
{
    return MetodicDefines::MetUid_BoxerDodging;
}

QString BoxerDodgingTemplate::name()
{
    return MetodicDefines::MetName_BoxerDodging;
}

QWidget *BoxerDodgingTemplate::execute(QWidget *parent, const QJsonObject &params)
{
    auto *retval = new BoxerDodgingExecute(parent);
    parent->layout()->addWidget(retval);
    retval->setParams(params);
    return retval;
}

QWidget *BoxerDodgingTemplate::visualize(QWidget *parent, const QString &testUid)
{
//    auto *retval = new OctaedronVisualize(parent);
//    parent->layout()->addWidget(retval);
//    retval->setTest(testUid);
//    return retval;

    return nullptr;
}

bool BoxerDodgingTemplate::editParams(QWidget *parent, QJsonObject &params)
{
    auto dialog = new BoxerDodgingParamsDialog(parent);
    dialog->setParams(params);
    bool retval = false;
    if (dialog->exec() == QDialog::Accepted)
    {
        params = dialog->getParams();
        retval = true;
    }
    return retval;
}
