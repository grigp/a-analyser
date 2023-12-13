#include "stepoffsettemplate.h"

#include "aanalyserapplication.h"
#include "metodicdefines.h"
#include "stepoffsetparamsdialog.h"
#include "stepoffsetexecute.h"
#include "stepoffsetvisualize.h"
#include "stepoffsetcalculator.h"
#include "metodicsfactory.h"

#include <QLayout>

StepOffsetTemplate::StepOffsetTemplate(QObject *parent)
    : MetodicTemplate (parent)
{

}

QString StepOffsetTemplate::uid()
{
    return MetodicDefines::MetUid_StepOffset;
}

QString StepOffsetTemplate::name()
{
    return MetodicDefines::MetName_StepOffset;
}

QWidget *StepOffsetTemplate::execute(QWidget *parent, const QJsonObject &params)
{
    auto *retval = new StepOffsetExecute(parent);
    parent->layout()->addWidget(retval);
    retval->setParams(params);
    return retval;
}

QWidget *StepOffsetTemplate::visualize(QWidget *parent, const QString &testUid)
{
    auto *retval = new StepOffsetVisualize(parent);
    parent->layout()->addWidget(retval);
    retval->setTest(testUid);
    return retval;
}

void StepOffsetTemplate::paintPreview(QPainter *painter, QRect &rect, const QString &testUid)
{
    Q_UNUSED(painter);
    Q_UNUSED(rect);
    Q_UNUSED(testUid);

    MetodicsFactory *metFactory = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics();
    auto calculator = metFactory->getCalculator(testUid);
    if (!calculator)
    {
        calculator = new StepOffsetCalculator(testUid, nullptr);
        calculator->calculate();
        metFactory->storeCalculator(testUid, calculator);
    }

    StepOffsetVisualize::paintPreview(painter, rect, testUid, static_cast<StepOffsetCalculator*>(calculator));
}

void StepOffsetTemplate::print(QPrinter *printer, const QString &testUid)
{
    StepOffsetVisualize::print(printer, testUid);
}

bool StepOffsetTemplate::editParams(QWidget *parent, QJsonObject &params)
{
    auto dialog = new StepOffsetParamsDialog(parent);
    dialog->setParams(params);
    bool retval = false;
    if (dialog->exec() == QDialog::Accepted)
    {
        params = dialog->getParams();
        retval = true;
    }
    return retval;
}
