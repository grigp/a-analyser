#include "trenagertemplate.h"

#include "metodicdefines.h"
#include "trenagerexecute.h"

#include <QLayout>

TrenagerTemplate::TrenagerTemplate(QObject *parent)
    : MetodicTemplate(parent)
{

}

QString TrenagerTemplate::uid()
{
    return MetodicDefines::MetUid_Trenager;
}

QString TrenagerTemplate::name()
{
    return MetodicDefines::MetName_Trenager;
}

QWidget *TrenagerTemplate::execute(QWidget *parent, const QJsonObject &params)
{
    auto *retval = new TrenagerExecute(parent);
    parent->layout()->addWidget(retval);
    retval->setParams(params);
    return retval;
}

QWidget *TrenagerTemplate::visualize(QWidget *parent, const QString &testUid)
{

}

bool TrenagerTemplate::editParams(QWidget *parent, QJsonObject &params)
{

}
