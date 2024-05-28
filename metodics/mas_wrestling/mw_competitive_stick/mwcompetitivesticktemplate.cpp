#include "mwcompetitivesticktemplate.h"

#include <QLayout>
#include <QDialog>

#include "aanalyserapplication.h"
#include "metodicsfactory.h"
#include "metodicdefines.h"
#include "mwcompetitivestickexecute.h"
#include "mwcompetitivestickcalculator.h"
#include "mwcompetitivestickvisualize.h"
#include "mwcompetitivestickparamsdialog.h"

MWCompetitiveStickTemplate::MWCompetitiveStickTemplate(QObject *parent)
    : MetodicTemplate (parent)
{

}

QString MWCompetitiveStickTemplate::uid()
{
    return MetodicDefines::MetUid_MWCompetitiveStick;
}

QString MWCompetitiveStickTemplate::name()
{
    return MetodicDefines::MetName_MWCompetitiveStick;
}

QWidget *MWCompetitiveStickTemplate::execute(QWidget *parent, const QJsonObject &params)
{
    auto *retval = new MWCompetitiveStickExecute(parent);
    parent->layout()->addWidget(retval);
    retval->setParams(params);
    return retval;
}

QWidget *MWCompetitiveStickTemplate::visualize(QWidget *parent, const QString &testUid)
{
    auto *retval = new MWCompetitiveStickVisualize(parent);
    parent->layout()->addWidget(retval);
    retval->setTest(testUid);
    return retval;
}

void MWCompetitiveStickTemplate::paintPreview(QPainter *painter, QRect &rect, const QString &testUid)
{
    MetodicsFactory *metFactory = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics();
    auto calculator = metFactory->getCalculator(testUid);
    if (!calculator)
    {
        calculator = new MWCompetitiveStickCalculator(testUid, nullptr);
        calculator->calculate();
        metFactory->storeCalculator(testUid, calculator);
    }

    MWCompetitiveStickVisualize::paintPreview(painter, rect, testUid, static_cast<MWCompetitiveStickCalculator*>(calculator));
}

void MWCompetitiveStickTemplate::print(QPrinter *printer, const QString &testUid)
{
    MWCompetitiveStickVisualize::print(printer, testUid);
}

bool MWCompetitiveStickTemplate::editParams(QWidget *parent, QJsonObject &params)
{
    auto dialog = new MWCompetitiveStickParamsDialog(parent);
    dialog->setParams(params);
    bool retval = false;
    if (dialog->exec() == QDialog::Accepted)
    {
        params = dialog->getParams();
        retval = true;
    }
    return retval;
}
