#include "aanalyserbuild.h"

#include "aanalyserapplication.h"
#include "connection.h"

#include "metodictemplate.h"
#include "stabtesttemplate.h"
#include "jumptesttemplate.h"
#include "jumpheighttesttemplate.h"
#include "teppingtesttemplate.h"
#include "droptesttemplate.h"

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

#include "skgvisual.h"
#include "stabilogramvisual.h"
#include "balistogramvisual.h"
#include "vectoranalysisvisual.h"
#include "histogramvisual.h"
#include "spectrstabvisual.h"
#include "sectiongraphvisual.h"

#include "filtersignal.h"

#include "stabilan01.h"
#include "jumpplate.h"

QList<MetodicTemplate *> AAnalyserBuild::getBuildTemplates(QObject *parent)
{
    return QList<MetodicTemplate *>()
            << new StabTestTemplate(parent)
            << new JumpTestTemplate(parent)
            << new JumpHeightTestTemplate(parent)
            << new TeppingTestTemplate(parent)
            << new DropTestTemplate(parent);
}

void AAnalyserBuild::registerFactors()
{
    auto* app = static_cast<AAnalyserApplication*>(QApplication::instance());
    app->registerGroup(new ClassicFactorsDescriptor(BaseDefines::tlChannel));
    app->registerGroup(new VectorFactorsDescriptor(BaseDefines::tlChannel));
    app->registerGroup(new RatioProbesFactorsDescriptor(BaseDefines::tlTest));
    app->registerGroup(new JumpHeightSingleFactorsDescriptor(BaseDefines::tlChannel));
    app->registerGroup(new JumpHeightFactorsDescriptor(BaseDefines::tlChannel));
    app->registerGroup(new TeppingTestFactorsDescriptor(BaseDefines::tlChannel));
    app->registerGroup(new DropTestFactorsDescriptor(BaseDefines::tlChannel));
    app->registerGroup(new HoppingFactorsDescriptor(BaseDefines::tlChannel));
    app->registerGroup(new IDSFactorsDescriptor(BaseDefines::tlChannel));
    app->registerGroup(new SpectrStabFactorsDescriptor(BaseDefines::tlChannel));
    app->registerGroup(new TargetFactorsDescriptor(BaseDefines::tlChannel));
}

void AAnalyserBuild::registerVisuals()
{
    auto* app = static_cast<AAnalyserApplication*>(QApplication::instance());
    app->registerVisual(new SKGVisual(BaseDefines::tlChannel));
    app->registerVisual(new StabilogramVisual(BaseDefines::tlChannel));
    app->registerVisual(new BalistogramVisual(BaseDefines::tlChannel));
    app->registerVisual(new VectorAnalysisVisual(BaseDefines::tlChannel));
    app->registerVisual(new HistogramVisual(BaseDefines::tlChannel));
    app->registerVisual(new SpectrStabVisual(BaseDefines::tlChannel));
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
    if (drvUid == JumpPlate::uid())
        return JumpPlate::getPorts();

    return QList<DeviceProtocols::Ports>();
}

QStringList AAnalyserBuild::getDriverProtocols(const QString &drvUid)
{
    //! Надо хардкодить все драйвера
    if (drvUid == Stabilan01::uid())
        return Stabilan01::getProtocols();
    else
    if (drvUid == JumpPlate::uid())
        return JumpPlate::getProtocols();
    return QStringList();
}

bool AAnalyserBuild::editDriverParams(const QString &drvUid, QJsonObject &params)
{
    //! Надо хардкодить все драйвера
    if (drvUid == Stabilan01::uid())
        return Stabilan01::editParams(params);
    else
    if (drvUid == JumpPlate::uid())
        return JumpPlate::editParams(params);
    return false;
}

Driver *AAnalyserBuild::createDriver(const QString &drvUid)
{
    //! Надо хардкодить все драйвера
    if (drvUid == Stabilan01::uid())
        return new Stabilan01();
    else
    if (drvUid == JumpPlate::uid())
        return new JumpPlate();
    return nullptr;
}

bool AAnalyserBuild::drvInitialSetup(const QString& uid,
                                     DeviceProtocols::Ports port,
                                     const QJsonObject &params,
                                     const QString &comment)
{
    if (uid == Stabilan01::uid())
        return Stabilan01::initialSetup(port, params, comment);
    return true;  //! По умолчанию true, ибо, если устройства нет, то настраивать нечего, значит удачно
}

bool AAnalyserBuild::isInitialSetup(const QString &drvUid)
{
    return (drvUid == Stabilan01::uid()); // || (drvUid == .....::uid());
}

bool AAnalyserBuild::isAutoRunInitialSetup()
{
    return true;
}

void AAnalyserBuild::assignDrivers(QMap<QString, QString> &drivers)
{
    //! Надо хардкодить все драйвера
    drivers.insert(Stabilan01::uid(), Stabilan01::name());
    drivers.insert(JumpPlate::uid(), JumpPlate::name());
}

QStringList AAnalyserBuild::getDefaultPPList()
{
    return QStringList();
}

bool AAnalyserBuild::isPPEnabled()
{
    return false;
}
