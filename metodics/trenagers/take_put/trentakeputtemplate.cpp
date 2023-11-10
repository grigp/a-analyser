#include "trentakeputtemplate.h"

#include "metodicdefines.h"
#include "trentakeputexecute.h"
#include "trenvisualize.h"
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
    auto *retval = new TrenVisualize(parent);
    parent->layout()->addWidget(retval);
    retval->setTest(testUid);
    return retval;
}

void TrenTakePutTemplate::paintPreview(QPainter *painter, QRect &rect, const QString &testUid)
{
    Q_UNUSED(painter);
    Q_UNUSED(rect);
    Q_UNUSED(testUid);
}

void TrenTakePutTemplate::print(QPrinter *printer, const QString &testUid)
{
    TrenVisualize::print(printer, testUid);
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
