#include "crosstemplate.h"

#include "aanalyserapplication.h"
#include "metodicdefines.h"
#include "crossparamsdialog.h"
#include "crossexecute.h"
#include "crossvisualize.h"
#include "crosscalculator.h"
#include "metodicsfactory.h"

#include <QLayout>
#include <QDebug>

CrossTemplate::CrossTemplate(QObject *parent)
    : MetodicTemplate (parent)
{

}

QString CrossTemplate::uid()
{
    return MetodicDefines::MetUid_Cross;
}

QString CrossTemplate::name()
{
    return MetodicDefines::MetName_Cross;
}

QWidget *CrossTemplate::execute(QWidget *parent, const QJsonObject &params)
{
    auto *retval = new CrossExecute(parent);
    parent->layout()->addWidget(retval);
    retval->setParams(params);
    return retval;
}

QWidget *CrossTemplate::visualize(QWidget *parent, const QString &testUid)
{
    auto *retval = new CrossVisualize(parent);
    parent->layout()->addWidget(retval);
    retval->setTest(testUid);
    return retval;
}

void CrossTemplate::paintPreview(QPainter *painter, QRect &rect, const QString &testUid)
{
    Q_UNUSED(painter);
    Q_UNUSED(rect);
    Q_UNUSED(testUid);

    MetodicsFactory *metFactory = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics();
    auto calculator = metFactory->getCalculator(testUid);
    if (!calculator)
    {
        calculator = new CrossCalculator(testUid, nullptr);
        calculator->calculate();
        metFactory->storeCalculator(testUid, calculator);
    }

    CrossVisualize::paintPreview(painter, rect, testUid, static_cast<CrossCalculator*>(calculator));

}

void CrossTemplate::print(QPrinter *printer, const QString &testUid)
{
    CrossVisualize::print(printer, testUid);
}

bool CrossTemplate::editParams(QWidget *parent, QJsonObject &params, const QString& testName)
{
    Q_UNUSED(testName);
    auto dialog = new CrossParamsDialog(parent);
    dialog->setParams(params);
    bool retval = false;
    if (dialog->exec() == QDialog::Accepted)
    {
        params = dialog->getParams();
        retval = true;
    }
    return retval;
}
