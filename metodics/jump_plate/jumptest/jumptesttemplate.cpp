#include "jumptesttemplate.h"

#include "metodicdefines.h"
#include "jumptestexecute.h"
#include "jumptestvisualize.h"
#include "jumptestparamsdialog.h"

#include <QLayout>

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
    parent->layout()->addWidget(retval);
    retval->setParams(params);
    return retval;
}

QWidget *JumpTestTemplate::visualize(QWidget *parent, const QString &testUid)
{
    auto *retval = new JumpTestVisualize(parent);
    parent->layout()->addWidget(retval);
    retval->setTest(testUid);
    return retval;
}

void JumpTestTemplate::paintPreview(QPainter *painter, QRect &rect, const QString &testUid)
{
    Q_UNUSED(painter);
    Q_UNUSED(rect);
    Q_UNUSED(testUid);
}

void JumpTestTemplate::print(QPrinter *printer, const QString &testUid)
{
    Q_UNUSED(printer);
    Q_UNUSED(testUid);
}

bool JumpTestTemplate::editParams(QWidget *parent, QJsonObject &params, const QString& testName)
{
    Q_UNUSED(testName);
    auto dialog = new JumpTestParamsDialog(parent);
    dialog->setParams(params);
    bool retval = false;
    if (dialog->exec() == QDialog::Accepted)
    {
        params = dialog->getParams();
        retval = true;
    }
    return retval;
}
