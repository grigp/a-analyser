#include "jumptesttemplate.h"

#include "metodicdefines.h"
#include "jumptestexecute.h"
#include "jumptestvisualize.h"

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

bool JumpTestTemplate::editParams(QJsonObject &params)
{

}
