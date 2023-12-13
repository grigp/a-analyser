#include "evolventatemplate.h"

#include <QLayout>

#include "aanalyserapplication.h"
#include "metodicdefines.h"
#include "metodicsfactory.h"
#include "evolventaparamsdialog.h"
#include "evolventaexecute.h"
#include "evolventavisualize.h"
#include "evolventacalculator.h"

EvolventaTemplate::EvolventaTemplate(QObject *parent)
    : MetodicTemplate (parent)
{

}

QString EvolventaTemplate::uid()
{
    return MetodicDefines::MetUid_Evolventa;
}

QString EvolventaTemplate::name()
{
    return MetodicDefines::MetName_Evolventa;
}

QWidget *EvolventaTemplate::execute(QWidget *parent, const QJsonObject &params)
{
    auto *retval = new EvolventaExecute(parent);
    parent->layout()->addWidget(retval);
    retval->setParams(params);
    return retval;
}

QWidget *EvolventaTemplate::visualize(QWidget *parent, const QString &testUid)
{
    auto *retval = new EvolventaVisualize(parent);
    parent->layout()->addWidget(retval);
    retval->setTest(testUid);
    return retval;
}

void EvolventaTemplate::paintPreview(QPainter *painter, QRect &rect, const QString &testUid)
{
    Q_UNUSED(painter);
    Q_UNUSED(rect);
    Q_UNUSED(testUid);

    MetodicsFactory *metFactory = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics();
    auto calculator = metFactory->getCalculator(testUid);
    if (!calculator)
    {
        calculator = new EvolventaCalculator(testUid, nullptr);
        calculator->calculate();
        metFactory->storeCalculator(testUid, calculator);
    }

    EvolventaVisualize::paintPreview(painter, rect, testUid, static_cast<EvolventaCalculator*>(calculator));
}

void EvolventaTemplate::print(QPrinter *printer, const QString &testUid)
{
    EvolventaVisualize::print(printer, testUid);
    Q_UNUSED(printer);
    Q_UNUSED(testUid);
}

bool EvolventaTemplate::editParams(QWidget *parent, QJsonObject &params)
{
    auto dialog = new EvolventaParamsDialog(parent);
    dialog->setParams(params);
    bool retval = false;
    if (dialog->exec() == QDialog::Accepted)
    {
        params = dialog->getParams();
        retval = true;
    }
    return retval;
}
