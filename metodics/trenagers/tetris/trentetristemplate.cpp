#include "trentetristemplate.h"

#include <QLayout>

#include "metodicdefines.h"
#include "trentetrisparamsdialog.h"
#include "trentetrisexecute.h"
#include "trenvisualize.h"

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
    auto *retval = new TrenTetrisExecute(parent);
    retval->setParams(params);
    parent->layout()->addWidget(retval);
    return retval;
}

QWidget *TrenTetrisTemplate::visualize(QWidget *parent, const QString &testUid)
{
    auto *retval = new TrenVisualize(parent);
    parent->layout()->addWidget(retval);
    retval->setTest(testUid);
    return retval;
}

void TrenTetrisTemplate::paintPreview(QPainter *painter, QRect &rect, const QString &testUid)
{
    Q_UNUSED(painter);
    Q_UNUSED(rect);
    Q_UNUSED(testUid);
}

void TrenTetrisTemplate::print(QPrinter *printer, const QString &testUid)
{
    TrenVisualize::print(printer, testUid);
}

bool TrenTetrisTemplate::editParams(QWidget *parent, QJsonObject &params, const QString& testName)
{
    auto dialog = new TrenTetrisParamsDialog(parent);
    dialog->setWindowTitle(tr("Параметры методики") + " - " + testName);
    dialog->setParams(params);
    bool retval = false;
    if (dialog->exec() == QDialog::Accepted)
    {
        params = dialog->getParams();
        retval = true;
    }
    return retval;
}
