#include "extend3dgamestemplate.h"

#include "dataprovider.h"
#include "metodicdefines.h"
#include "testcalculator.h"
#include "extend3dgamesparamsdialog.h"
#include "extend3dgamesexecute.h"
#include "trenvisualize.h"

#include <QLayout>
#include <QDebug>

Extend3DGamesTemplate::Extend3DGamesTemplate(QObject *parent)
    : MetodicTemplate(parent)
{

}

QString Extend3DGamesTemplate::uid()
{
    return MetodicDefines::MetUid_Extend3DGamesTest;
}

QString Extend3DGamesTemplate::name()
{
    return MetodicDefines::MetName_Extend3DGamesTest;
}

QWidget *Extend3DGamesTemplate::execute(QWidget *parent, const QJsonObject &params)
{
    auto *retval = new Extend3DGamesExecute(parent);
    parent->layout()->addWidget(retval);
    retval->run(params);
    return retval;
}

QWidget *Extend3DGamesTemplate::visualize(QWidget *parent, const QString &testUid)
{
    auto *retval = new TrenVisualize(parent);
    parent->layout()->addWidget(retval);
    retval->setTest(testUid);
    return retval;
}

void Extend3DGamesTemplate::paintPreview(QPainter *painter, QRect &rect, const QString &testUid)
{
    Q_UNUSED(painter);
    Q_UNUSED(rect);
    Q_UNUSED(testUid);
}

void Extend3DGamesTemplate::print(QPrinter *printer, const QString &testUid)
{
    TrenVisualize::print(printer, testUid);
}

bool Extend3DGamesTemplate::editParams(QWidget *parent, QJsonObject &params)
{
    auto dialog = new Extend3DGamesParamsDialog(parent);
    dialog->setParams(params);
    bool retval = false;
    if (dialog->exec() == QDialog::Accepted)
    {
        params = dialog->getParams();
        retval = true;
    }
    return retval;
}

TestCalculator *Extend3DGamesTemplate::calculator(const QString &testUid)
{
    Q_UNUSED(testUid);
//    DataDefines::TestInfo ti;
//    if (DataProvider::getTestInfo(testUid, ti))
//    {
//        auto params = ti.params;
//        auto cnd = params["condition"].toInt();

//        if (cnd == 0)
//            return new StabSignalsTestCalculator(testUid);
//        else
//        if (cnd == 1)
//            return new StateChampionsTestCalculator(testUid);
//        else
//        if (cnd == 2)
//            return new DopuskCalculator(testUid);
//        else
//        if (cnd == 3)
//            return new IDSCalculator(testUid);
//        else
//        if (cnd == 4)
//            return new TargetCalculator(testUid);
//        else
//        if (cnd == 5)
//            return new StressStrategyCalculator(testUid);
//    }
    return nullptr;
}

