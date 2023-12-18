#include "aanalyserbuild.h"

#include "aanalyserapplication.h"

#include "metodictemplate.h"
#include "stabtesttemplate.h"
#include "extend3dgamestemplate.h"
#include "jumptesttemplate.h"
#include "jumpheighttesttemplate.h"
#include "teppingtesttemplate.h"
#include "trentakeputtemplate.h"
#include "trentetristemplate.h"
#include "droptesttemplate.h"
#include "crosstemplate.h"
#include "stepoffsettemplate.h"
#include "stepdeviationtemplate.h"
#include "triangletemplate.h"
#include "evolventatemplate.h"
#include "octaedrontemplate.h"
#include "boxerdodgingtemplate.h"

#include "classicfactorsdescriptor.h"
#include "vectorfactorsdescriptor.h"
#include "ratioprobesfactorsdescriptor.h"
#include "jumpheightsinglefactorsdescriptor.h"
#include "jumpheightfactorsdescriptor.h"
#include "teppingtestfactorsdescriptor.h"
#include "droptestfactorsdescriptor.h"
#include "hoppingfactorsdescriptor.h"
#include "idsfactorsdescriptor.h"
#include "spectrstabfactorsdescriptor.h"
#include "targetfactorsdescriptor.h"
#include "trenresultfactorsdescriptor.h"
#include "crossfactorsdescriptor.h"
#include "stepoffsetfactorsdescriptor.h"
#include "evolventafactorsdescriptor.h"
#include "octaedronfactorsdescritor.h"
#include "boxerdodgingmultifactordescriptor.h"
#include "stepdeviationfactorsdescriptor.h"
#include "trianglefactorsdescriptor.h"
#include "triangleconslutionfactorsdescriptor.h"

#include "pulsefactorsdescriptor.h"
#include "pulsespectrfactorsdescriptor.h"

#include "skgvisual.h"
#include "stabilogramvisual.h"
#include "histogramvisual.h"
#include "spectrstabvisual.h"
#include "balistogramvisual.h"
#include "vectoranalysisvisual.h"
#include "bilateralvisual.h"
#include "dynamosignalvisual.h"
#include "myogramsignalvisual.h"
#include "ritmogramvisual.h"
#include "variationpulsogramvisual.h"
#include "korrelationritmogramvisual.h"
#include "autocorrelationanalysisvisual.h"
#include "spectrpulsevisual.h"
#include "sectiongraphvisual.h"

#include "mancoordppvisual.h"
#include "commonppvisual.h"

#include "filtersignal.h"

#include "stabilan01.h"
#include "amedplatform01.h"
#include "bilateral.h"
#include "jumpplate.h"

QList<MetodicTemplate *> AAnalyserBuild::getBuildTemplates(QObject *parent)
{
    return QList<MetodicTemplate *>()
            << new StabTestTemplate(parent)
            << new Extend3DGamesTemplate(parent)
            << new JumpTestTemplate(parent)
            << new JumpHeightTestTemplate(parent)
            << new TeppingTestTemplate(parent)
            << new DropTestTemplate(parent)
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
    auto* app = static_cast<AAnalyserApplication*>(QApplication::instance());
    app->registerGroup(new ClassicFactorsDescriptor(BaseDefines::tlChannel));
    app->registerGroup(new VectorFactorsDescriptor(BaseDefines::tlChannel));
    app->registerGroup(new IDSFactorsDescriptor(BaseDefines::tlChannel));
    app->registerGroup(new SpectrStabFactorsDescriptor(BaseDefines::tlChannel));
    app->registerGroup(new RatioProbesFactorsDescriptor(BaseDefines::tlTest));
    app->registerGroup(new TargetFactorsDescriptor(BaseDefines::tlProbe));
    app->registerGroup(new TrenResultFactorsDescriptor(BaseDefines::tlProbe));
    app->registerGroup(new BoxerDodgingMultifactorDescriptor(BaseDefines::tlProbe));
    app->registerGroup(new CrossFactorsDescriptor(BaseDefines::tlProbe));
    app->registerGroup(new EvolventaFactorsDescriptor(BaseDefines::tlProbe));
    app->registerGroup(new OctaedronFactorsDescritior(BaseDefines::tlProbe));
    app->registerGroup(new StepDeviationFactorsDescriptor(BaseDefines::tlProbe));
    app->registerGroup(new StepOffsetFactorsDescriptor(BaseDefines::tlProbe));
    app->registerGroup(new TriangleFactorsDescriptor(BaseDefines::tlProbe));
    app->registerGroup(new TriangleConslutionFactorsDescriptor(BaseDefines::tlProbe));

    app->registerGroup(new PulseFactorsDescriptor(BaseDefines::tlChannel));
    app->registerGroup(new PulseSpectrFactorsDescriptor(BaseDefines::tlChannel));

    app->registerGroup(new DropTestFactorsDescriptor(BaseDefines::tlChannel));
    app->registerGroup(new HoppingFactorsDescriptor(BaseDefines::tlChannel));
    app->registerGroup(new JumpHeightFactorsDescriptor(BaseDefines::tlChannel));
    app->registerGroup(new JumpHeightSingleFactorsDescriptor(BaseDefines::tlChannel));
    app->registerGroup(new TeppingTestFactorsDescriptor(BaseDefines::tlChannel));
}

void AAnalyserBuild::registerVisuals()
{
    auto* app = static_cast<AAnalyserApplication*>(QApplication::instance());
    app->registerVisual(new SKGVisual(BaseDefines::tlChannel));
    app->registerVisual(new StabilogramVisual(BaseDefines::tlChannel));
    app->registerVisual(new HistogramVisual(BaseDefines::tlChannel));
    app->registerVisual(new SpectrStabVisual(BaseDefines::tlChannel));
    app->registerVisual(new VectorAnalysisVisual(BaseDefines::tlChannel));
    app->registerVisual(new BalistogramVisual(BaseDefines::tlChannel));
    app->registerVisual(new MyogramSignalVisual(BaseDefines::tlChannel));
    app->registerVisual(new DynamoSignalVisual(BaseDefines::tlChannel));
    app->registerVisual(new RitmogramVisual(BaseDefines::tlChannel));
    app->registerVisual(new VariationPulsogramVisual(BaseDefines::tlChannel));
    app->registerVisual(new KorrelationRitmogramVisual(BaseDefines::tlChannel));
    app->registerVisual(new AutoCorrelationAnalysisVisual(BaseDefines::tlChannel));
    app->registerVisual(new SpectrPulseVisual(BaseDefines::tlChannel));
    app->registerVisual(new BilateralVisual(BaseDefines::tlProbe));
    app->registerVisual(new SectionGraphVisual(BaseDefines::tlSection));
}

void AAnalyserBuild::registerPPVisuals()
{
    auto* app = static_cast<AAnalyserApplication*>(QApplication::instance());
    app->registerPPVisual(new ManCoordPPVisual());
    app->registerPPVisual(new CommonPPVisual());
}

void AAnalyserBuild::registerSignalTransformers()
{
    auto* app = static_cast<AAnalyserApplication*>(QApplication::instance());
    app->registerSignalTransformer(new FilterSignal());
}

QList<DeviceProtocols::Ports> AAnalyserBuild::getDriverPorts(const QString &drvUid)
{
    //! Надо хардкодить все драйвера
    if (drvUid == Stabilan01::uid())
        return Stabilan01::getPorts();
    else
    if (drvUid == AMedPlatform01::uid())
        return AMedPlatform01::getPorts();
    else
    if (drvUid == JumpPlate::uid())
        return JumpPlate::getPorts();
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
    if (drvUid == AMedPlatform01::uid())
        return AMedPlatform01::getProtocols();
    else
    if (drvUid == JumpPlate::uid())
        return JumpPlate::getProtocols();
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
    if (drvUid == AMedPlatform01::uid())
        return AMedPlatform01::editParams(params);
    else
    if (drvUid == JumpPlate::uid())
        return JumpPlate::editParams(params);
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
    if (drvUid == AMedPlatform01::uid())
        return new AMedPlatform01();
    else
    if (drvUid == JumpPlate::uid())
        return new JumpPlate();
    else
    if (drvUid == Bilateral::uid())
        return new Bilateral();
    return nullptr;
}


void AAnalyserBuild::assignDrivers(QMap<QString, QString> &drivers)
{
    //! Надо хардкодить все драйвера
    drivers.insert(Stabilan01::uid(), Stabilan01::name());
    drivers.insert(AMedPlatform01::uid(), AMedPlatform01::name());
    drivers.insert(JumpPlate::uid(), JumpPlate::name());
    drivers.insert(Bilateral::uid(), Bilateral::name());
}



QStringList AAnalyserBuild::getDefaultPPList()
{
    return QStringList() << "neurology";
}

bool AAnalyserBuild::isPPEnabled()
{
    return true;
}

