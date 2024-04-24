#include "aanalyserbuild.h"

#include "aanalyserapplication.h"
#include "connection.h"

#include "metodictemplate.h"
#include "stabtesttemplate.h"

#include "classicfactorsdescriptor.h"
#include "vectorfactorsdescriptor.h"
#include "ratioprobesfactorsdescriptor.h"
#include "idsfactorsdescriptor.h"
#include "spectrstabfactorsdescriptor.h"
#include "targetfactorsdescriptor.h"

#include "pulsefactorsdescriptor.h"
#include "pulsespectrfactorsdescriptor.h"

#include "skgvisual.h"
#include "stabilogramvisual.h"
#include "balistogramvisual.h"
#include "vectoranalysisvisual.h"
#include "histogramvisual.h"
#include "spectrstabvisual.h"
#include "bilateralvisual.h"
#include "dynamosignalvisual.h"
#include "myogramsignalvisual.h"
#include "ritmogramvisual.h"
#include "variationpulsogramvisual.h"
#include "korrelationritmogramvisual.h"
#include "autocorrelationanalysisvisual.h"
#include "spectrpulsevisual.h"
#include "sectiongraphvisual.h"

#include "filtersignal.h"

#include "stabilan01.h"
#include "bilateral.h"

QList<MetodicTemplate *> AAnalyserBuild::getBuildTemplates(QObject *parent)
{
    return QList<MetodicTemplate *>()
            << new StabTestTemplate(parent);
}

void AAnalyserBuild::registerFactors()
{
    auto* app = static_cast<AAnalyserApplication*>(QApplication::instance());
    app->registerGroup(new ClassicFactorsDescriptor(BaseDefines::tlChannel));
    app->registerGroup(new VectorFactorsDescriptor(BaseDefines::tlChannel));
    app->registerGroup(new RatioProbesFactorsDescriptor(BaseDefines::tlTest));
    app->registerGroup(new IDSFactorsDescriptor(BaseDefines::tlChannel));
    app->registerGroup(new SpectrStabFactorsDescriptor(BaseDefines::tlChannel));
    app->registerGroup(new TargetFactorsDescriptor(BaseDefines::tlChannel));

    app->registerGroup(new PulseFactorsDescriptor(BaseDefines::tlChannel));
    app->registerGroup(new PulseSpectrFactorsDescriptor(BaseDefines::tlChannel));
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
    app->registerVisual(new HistogramVisual(BaseDefines::tlChannel));
    app->registerVisual(new SpectrStabVisual(BaseDefines::tlChannel));
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
//    auto* app = static_cast<AAnalyserApplication*>(QApplication::instance());
//    app->registerPPVisual(new ManCoordPPVisual());
//    app->registerPPVisual(new CommonPPVisual());
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

bool AAnalyserBuild::drvInitialSetup(Connection &connection)
{
    if (connection.driverUid() == Stabilan01::uid())
        return Stabilan01::initialSetup(connection);
    return true;  //! По умолчанию true, ибо, если устройства нет, то настраивать нечего, значит удачно
}

bool AAnalyserBuild::isInitialSetup(const QString &drvUid)
{
    return (drvUid == Stabilan01::uid()); // || (drvUid == .....::uid());
}

bool AAnalyserBuild::isAutoRunInitialSetup()
{
    return false;
}

void AAnalyserBuild::assignDrivers(QMap<QString, QString> &drivers)
{
    //! Надо хардкодить все драйвера
    drivers.insert(Stabilan01::uid(), Stabilan01::name());
    drivers.insert(Bilateral::uid(), Bilateral::name());
}

QStringList AAnalyserBuild::getDefaultPPList()
{
    return QStringList();
}

bool AAnalyserBuild::isPPEnabled()
{
    return false;
}

