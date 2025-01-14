#include "droptesttemplate.h"

#include "droptestexecute.h"
#include "droptestparamsdialog.h"
#include "droptestvisualize.h"

#include "metodicdefines.h"

#include <QLayout>

DropTestTemplate::DropTestTemplate(QObject *parent)
    : MetodicTemplate(parent)
{

}

QString DropTestTemplate::uid()
{
    return MetodicDefines::MetUid_DropTest;
}

QString DropTestTemplate::name()
{
    return MetodicDefines::MetName_DropTest;
}

void DropTestTemplate::print(QPrinter *printer, const QString &testUid)
{
    Q_UNUSED(printer);
    Q_UNUSED(testUid);
}

QWidget *DropTestTemplate::execute(QWidget *parent, const QJsonObject &params)
{
    auto *retval = new DropTestExecute(parent);
    parent->layout()->addWidget(retval);
    retval->setParams(params);
    return retval;
}

QWidget *DropTestTemplate::visualize(QWidget *parent, const QString &testUid)
{
    auto *retval = new DropTestVisualize(parent);
    parent->layout()->addWidget(retval);
    retval->setTest(testUid);
    return retval;
}

void DropTestTemplate::paintPreview(QPainter *painter, QRect &rect, const QString &testUid)
{
    Q_UNUSED(painter);
    Q_UNUSED(rect);
    Q_UNUSED(testUid);
}

bool DropTestTemplate::editParams(QWidget *parent, QJsonObject &params, const QString& testName)
{
    Q_UNUSED(testName);
    auto dialog = new DropTestParamsDialog(parent);
    dialog->setParams(params);
    bool retval = false;
    if (dialog->exec() == QDialog::Accepted)
    {
        params = dialog->getParams();
        retval = true;
    }
    return retval;
}
