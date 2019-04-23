#include "jumptesttemplate.h"

#include "metodicdefines.h"
#include "jumptestexecute.h"
#include "jumptestvisualize.h"
#include "jumptestparamsdialog.h"

JumpTestTemplate::JumpTestTemplate(QObject *parent)
    : MetodicTemplate(parent)
{

}

QString JumpTestTemplate::uid()
{
    return MetodicDefines::MetUid_JumpTest;
}

QString JumpTestTemplate::name()
{
    return MetodicDefines::MetName_JumpTest;
}

QWidget *JumpTestTemplate::execute(QWidget *parent, const QJsonObject &params)
{
    auto *retval = new JumpTestExecute(parent);
    retval->setParams(params);
    return retval;
}

QWidget *JumpTestTemplate::visualize(QWidget *parent, const QJsonObject &params)
{
    auto *retval = new JumpTestVisualize(parent);
    retval->setParams(params);
    return retval;
}

bool JumpTestTemplate::editParams(QWidget *parent, QJsonObject &params)
{
    auto dialog = new JumpTestParamsDialog(parent);
    dialog->setParams(params);
    bool retval = false;
    connect(dialog, &JumpTestParamsDialog::accepted, this, [=]()
    {
        QJsonObject par = dialog->getParams();
        //todo: сохранить параметры
        //retval = true;
    });
    dialog->exec();
    return retval;
}
