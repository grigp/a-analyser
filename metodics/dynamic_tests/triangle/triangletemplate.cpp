#include "triangletemplate.h"

#include "aanalyserapplication.h"
#include "metodicdefines.h"
#include "metodicsfactory.h"
#include "triangleparamsdialog.h"
#include "triangleexecute.h"
#include "trianglevisualize.h"
#include "trianglecalculator.h"

#include <QLayout>

TriangleTemplate::TriangleTemplate(QObject *parent)
    : MetodicTemplate (parent)
{

}

QString TriangleTemplate::uid()
{
    return MetodicDefines::MetUid_Triangle;
}

QString TriangleTemplate::name()
{
    return MetodicDefines::MetName_Triangle;
}

QWidget *TriangleTemplate::execute(QWidget *parent, const QJsonObject &params)
{
    auto *retval = new TriangleExecute(parent);
    parent->layout()->addWidget(retval);
    retval->setParams(params);
    return retval;
}

QWidget *TriangleTemplate::visualize(QWidget *parent, const QString &testUid)
{
    auto *retval = new TriangleVisualize(parent);
    parent->layout()->addWidget(retval);
    retval->setTest(testUid);
    return retval;
}

void TriangleTemplate::paintPreview(QPainter *painter, QRect &rect, const QString &testUid)
{
    Q_UNUSED(painter);
    Q_UNUSED(rect);
    Q_UNUSED(testUid);

    MetodicsFactory *metFactory = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics();
    auto calculator = metFactory->getCalculator(testUid);
    if (!calculator)
    {
        calculator = new TriangleCalculator(testUid, nullptr);
        calculator->calculate();
        metFactory->storeCalculator(testUid, calculator);
    }

    TriangleVisualize::paintPreview(painter, rect, testUid, static_cast<TriangleCalculator*>(calculator));
}

void TriangleTemplate::print(QPrinter *printer, const QString &testUid)
{
    TriangleVisualize::print(printer, testUid);
}

bool TriangleTemplate::editParams(QWidget *parent, QJsonObject &params)
{
    auto dialog = new TriangleParamsDialog(parent);
    dialog->setParams(params);
    bool retval = false;
    if (dialog->exec() == QDialog::Accepted)
    {
        params = dialog->getParams();
        retval = true;
    }
    return retval;
}
