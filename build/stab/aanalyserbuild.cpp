#include "aanalyserbuild.h"

#include "aanalyserapplication.h"

#include "metodictemplate.h"
#include "stabtesttemplate.h"
#include "extend3dgamestemplate.h"
#include "trentakeputtemplate.h"
#include "trentetristemplate.h"
#include "crosstemplate.h"
#include "stepoffsettemplate.h"
#include "stepdeviationtemplate.h"
#include "triangletemplate.h"
#include "evolventatemplate.h"
#include "octaedrontemplate.h"
#include "boxerdodgingtemplate.h"

#include "classicfactors.h"
#include "vectorfactors.h"
#include "ratioprobesfactors.h"
#include "idsfactors.h"
#include "targetfactors.h"
#include "trenresultfactors.h"
#include "crossfactors.h"
#include "stepoffsetfactors.h"
#include "evolventafactors.h"
#include "octaedronfactors.h"
#include "boxerdodgingmultifactor.h"
#include "stepdeviationfactors.h"
#include "trianglefactors.h"
#include "triangleconslutionfactors.h"

#include "skgvisual.h"
#include "stabilogramvisual.h"
#include "balistogramvisual.h"
#include "vectoranalysisvisual.h"
#include "bilateralvisual.h"
#include "dynamosignalvisual.h"
#include "myogramsignalvisual.h"

#include "stabilan01.h"
#include "bilateral.h"

QList<MetodicTemplate *> AAnalyserBuild::getBuildTemplates(QObject *parent)
{
    return QList<MetodicTemplate *>()
            << new StabTestTemplate(parent)
            << new Extend3DGamesTemplate(parent)
            << new TrenTetrisTemplate(parent)
            << new TrenTakePutTemplate(parent)
            << new CrossTemplate(parent)
            << new StepOffsetTemplate(parent)
            << new StepDeviationTemplate(parent)
            << new TriangleTemplate(parent)
            << new EvolventaTemplate(parent)
            << new OctaedronTemplate(parent)
            << new BoxerDodgingTemplate(parent);
}

void AAnalyserBuild::registerFactors()
{
    ClassicFactors::registerFactors();
    VectorFactors::registerFactors();
    RatioProbesFactors::registerFactors();
    IDSFactors::registerFactors();
    TargetFactors::registerFactors();
    TrenResultFactors::registerFactors();
    CrossFactors::registerFactors();
    StepOffsetFactors::registerFactors();
    EvolventaFactors::registerFactors();
    OctaedronFactors::registerFactors();
    BoxerDodgingMultifactor::registerFactors();
    StepDeviationFactors::registerFactors();
    TriangleFactors::registerFactors();
    TriangleConslutionFactors::registerFactors();
}

void AAnalyserBuild::registerVisuals()
{
    auto* app = static_cast<AAnalyserApplication*>(QApplication::instance());
    app->registerVisual(new SKGVisual(BaseDefines::tlChannel));
    app->registerVisual(new StabilogramVisual(BaseDefines::tlChannel));
    app->registerVisual(new BalistogramVisual(BaseDefines::tlChannel));
    app->registerVisual(new MyogramSignalVisual(BaseDefines::tlChannel));
    app->registerVisual(new DynamoSignalVisual(BaseDefines::tlChannel));
    app->registerVisual(new VectorAnalysisVisual(BaseDefines::tlChannel));
    app->registerVisual(new BilateralVisual(BaseDefines::tlProbe));
}

QList<DeviceProtocols::Ports> AAnalyserBuild::getDriverPorts(const QString &drvUid)
{
    //! Надо хардкодить все драйвера
    if (drvUid == Stabilan01::uid())
        return Stabilan01::getPorts();
    else
    if (drvUid == Bilateral::uid())
        return Bilateral::getPorts();

    return QList<DeviceProtocols::Ports>();
}

QStringList AAnalyserBuild::getDriverProtocols(const QString &drvUid)
{
    //! Надо хардкодить все драйвера
    if (drvUid == Stabilan01::uid())
        return Stabilan01::getProtocols();
    else
    if (drvUid == Bilateral::uid())
        return Bilateral::getProtocols();

    return QStringList();
}

bool AAnalyserBuild::editDriverParams(const QString &drvUid, QJsonObject &params)
{
    //! Надо хардкодить все драйвера
    if (drvUid == Stabilan01::uid())
        return Stabilan01::editParams(params);
    else
    if (drvUid == Bilateral::uid())
        return Bilateral::editParams(params);

    return false;
}

Driver *AAnalyserBuild::createDriver(const QString &drvUid)
{
    //! Надо хардкодить все драйвера
    if (drvUid == Stabilan01::uid())
        return new Stabilan01();
    else
    if (drvUid == Bilateral::uid())
        return new Bilateral();

    return nullptr;
}


void AAnalyserBuild::assignDrivers(QMap<QString, QString> &drivers)
{
    //! Надо хардкодить все драйвера
    drivers.insert(Stabilan01::uid(), Stabilan01::name());
    drivers.insert(Bilateral::uid(), Bilateral::name());
}
