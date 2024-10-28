#include "teppingtesttemplate.h"

#include "metodicdefines.h"
#include "teppingtestexecute.h"
#include "teppingtestvisualize.h"
#include "teppingtestparamsdialog.h"

#include <QLayout>

TeppingTestTemplate::TeppingTestTemplate(QObject *parent)
    : MetodicTemplate(parent)
{

}

QString TeppingTestTemplate::uid()
{
    return MetodicDefines::MetUid_TeppingTest;
}

QString TeppingTestTemplate::name()
{
    return MetodicDefines::MetName_TeppingTest;
}

QWidget *TeppingTestTemplate::execute(QWidget *parent, const QJsonObject &params)
{
    auto *retval = new TeppingTestExecute(parent);
    parent->layout()->addWidget(retval);
    retval->setParams(params);
    return retval;
}

QWidget *TeppingTestTemplate::visualize(QWidget *parent, const QString &testUid)
{
    auto *retval = new TeppingTestVisualize(parent);
    parent->layout()->addWidget(retval);
    retval->setTest(testUid);
    return retval;
}

void TeppingTestTemplate::paintPreview(QPainter *painter, QRect &rect, const QString &testUid)
{
    Q_UNUSED(painter);
    Q_UNUSED(rect);
    Q_UNUSED(testUid);
}

void TeppingTestTemplate::print(QPrinter *printer, const QString &testUid)
{
    Q_UNUSED(printer);
    Q_UNUSED(testUid);
}

bool TeppingTestTemplate::editParams(QWidget *parent, QJsonObject &params, const QString& testName)
{
    Q_UNUSED(testName);
    auto dialog = new TeppingTestParamsDialog(parent);
    dialog->setParams(params);
    bool retval = false;
    if (dialog->exec() == QDialog::Accepted)
    {
        params = dialog->getParams();
        retval = true;
    }
    return retval;
}
