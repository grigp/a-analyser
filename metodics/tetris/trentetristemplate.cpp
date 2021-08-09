#include "trentetristemplate.h"

#include "metodicdefines.h"
#include "trentetrisparamsdialog.h"

TrenTetrisTemplate::TrenTetrisTemplate(QObject *parent)
    : MetodicTemplate (parent)
{

}

QString TrenTetrisTemplate::uid()
{
    return MetodicDefines::MetUid_TrenTetris;
}

QString TrenTetrisTemplate::name()
{
    return MetodicDefines::MetName_TrenTetris;
}

QWidget *TrenTetrisTemplate::execute(QWidget *parent, const QJsonObject &params)
{

}

QWidget *TrenTetrisTemplate::visualize(QWidget *parent, const QString &testUid)
{

}

bool TrenTetrisTemplate::editParams(QWidget *parent, QJsonObject &params)
{
    auto dialog = new TrenTetrisParamsDialog(parent);
    dialog->setParams(params);
    bool retval = false;
    if (dialog->exec() == QDialog::Accepted)
    {
        params = dialog->getParams();
        retval = true;
    }
    return retval;
}
