#include "jumpheighttesttemplate.h"

#include "metodicdefines.h"
#include "jumpheighttestexecute.h"
#include "jumpheighttestvisualize.h"
#include "jumpheighttestparamsdialog.h"

#include <QLayout>

JumpHeightTestTemplate::JumpHeightTestTemplate(QObject *parent)
    : MetodicTemplate(parent)
{

}

QString JumpHeightTestTemplate::uid()
{
    return MetodicDefines::MetUid_JumpHeightTest;
}

QString JumpHeightTestTemplate::name()
{
    return MetodicDefines::MetName_JumpHeightTest;
}

QWidget *JumpHeightTestTemplate::execute(QWidget *parent, const QJsonObject &params)
{
    auto *retval = new JumpHeightTestExecute(parent);
    parent->layout()->addWidget(retval);
    retval->setParams(params);
    return retval;
}

QWidget *JumpHeightTestTemplate::visualize(QWidget *parent, const QString &testUid)
{
    auto *retval = new JumpHeightTestVisualize(parent);
    parent->layout()->addWidget(retval);
    retval->setTest(testUid);
    return retval;
}

void JumpHeightTestTemplate::paintPreview(QPainter *painter, QRect &rect, const QString &testUid)
{
    Q_UNUSED(painter);
    Q_UNUSED(rect);
    Q_UNUSED(testUid);
}

void JumpHeightTestTemplate::print(QPrinter *printer, const QString &testUid)
{
    Q_UNUSED(printer);
    Q_UNUSED(testUid);
}

bool JumpHeightTestTemplate::editParams(QWidget *parent, QJsonObject &params, const QString& testName)
{
    Q_UNUSED(testName);
    auto dialog = new JumpHeightTestParamsDialog(parent);
    dialog->setParams(params);
    bool retval = false;
    if (dialog->exec() == QDialog::Accepted)
    {
        params = dialog->getParams();
        retval = true;
    }
    return retval;
}


