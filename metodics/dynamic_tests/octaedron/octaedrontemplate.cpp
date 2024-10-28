#include "octaedrontemplate.h"

#include <QLayout>

#include "metodicdefines.h"
#include "octaedronparamsdialog.h"
#include "octaedronexecute.h"
#include "octaedronvisualize.h"

OctaedronTemplate::OctaedronTemplate(QObject *parent)
    : MetodicTemplate (parent)
{

}

QString OctaedronTemplate::uid()
{
    return MetodicDefines::MetUid_Octaedron;
}

QString OctaedronTemplate::name()
{
    return MetodicDefines::MetName_Octaedron;
}

QWidget *OctaedronTemplate::execute(QWidget *parent, const QJsonObject &params)
{
    auto *retval = new OctaedronExecute(parent);
    parent->layout()->addWidget(retval);
    retval->setParams(params);
    return retval;
}

QWidget *OctaedronTemplate::visualize(QWidget *parent, const QString &testUid)
{
    auto *retval = new OctaedronVisualize(parent);
    parent->layout()->addWidget(retval);
    retval->setTest(testUid);
    return retval;
}

void OctaedronTemplate::paintPreview(QPainter *painter, QRect &rect, const QString &testUid)
{
    Q_UNUSED(painter);
    Q_UNUSED(rect);
    Q_UNUSED(testUid);
}

void OctaedronTemplate::print(QPrinter *printer, const QString &testUid)
{
    OctaedronVisualize::print(printer, testUid);
}

bool OctaedronTemplate::editParams(QWidget *parent, QJsonObject &params, const QString& testName)
{
    Q_UNUSED(testName);
    auto dialog = new OctaedronParamsDialog(parent);
    dialog->setParams(params);
    bool retval = false;
    if (dialog->exec() == QDialog::Accepted)
    {
        params = dialog->getParams();
        retval = true;
    }
    return retval;
}
