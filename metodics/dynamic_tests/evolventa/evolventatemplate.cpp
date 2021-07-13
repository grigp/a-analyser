#include "evolventatemplate.h"

#include <QLayout>

#include "metodicdefines.h"
#include "evolventaparamsdialog.h"
#include "evolventaexecute.h"
#include "evolventavisualize.h"

EvolventaTemplate::EvolventaTemplate(QObject *parent)
    : MetodicTemplate (parent)
{

}

QString EvolventaTemplate::uid()
{
    return MetodicDefines::MetUid_Evolventa;
}

QString EvolventaTemplate::name()
{
    return MetodicDefines::MetName_Evolventa;
}

QWidget *EvolventaTemplate::execute(QWidget *parent, const QJsonObject &params)
{
    auto *retval = new EvolventaExecute(parent);
    parent->layout()->addWidget(retval);
    retval->setParams(params);
    return retval;
}

QWidget *EvolventaTemplate::visualize(QWidget *parent, const QString &testUid)
{
    auto *retval = new EvolventaVisualize(parent);
    parent->layout()->addWidget(retval);
    retval->setTest(testUid);
    return retval;
}

bool EvolventaTemplate::editParams(QWidget *parent, QJsonObject &params)
{
    auto dialog = new EvolventaParamsDialog(parent);
    dialog->setParams(params);
    bool retval = false;
    if (dialog->exec() == QDialog::Accepted)
    {
        params = dialog->getParams();
        retval = true;
    }
    return retval;
}
