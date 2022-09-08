#include "aanalyserbuild.h"

#include "aanalyserapplication.h"

#include "metodictemplate.h"
#include "bedsidescalestestertemplate.h"

#include "basedefines.h"
#include "devicesdefines.h"
#include "driver.h"
#include "bedsidescales01.h"

#include "balistogramvisual.h"
#include "weightplategraphvisual.h"
#include "wpgraphanalysisvisual.h"
#include "sectiongraphvisual.h"

#include "filtersignal.h"

QList<MetodicTemplate *> AAnalyserBuild::getBuildTemplates(QObject *parent)
{
    return QList<MetodicTemplate *>()
            << new BedsideScalesTesterTemplate(parent);
}

void AAnalyserBuild::registerFactors()
{
}

void AAnalyserBuild::registerVisuals()
{
    auto* app = static_cast<AAnalyserApplication*>(QApplication::instance());
    app->registerVisual(new BalistogramVisual(BaseDefines::tlChannel));
    app->registerVisual(new WeightPlateGraphVisual(BaseDefines::tlChannel));
    app->registerVisual(new WPGraphAnalysisVisual(BaseDefines::tlChannel));
    app->registerVisual(new SectionGraphVisual(BaseDefines::tlSection));
}

void AAnalyserBuild::registerSignalTransformers()
{
    auto* app = static_cast<AAnalyserApplication*>(QApplication::instance());
    app->registerSignalTransformer(new FilterSignal());
}

QList<DeviceProtocols::Ports> AAnalyserBuild::getDriverPorts(const QString &drvUid)
{
    //! Надо хардкодить все драйвера
    if (drvUid == DevicesDefines::uid_bedsidescales01)
        return BedsideScales01::getPorts();
    return QList<DeviceProtocols::Ports>();
}

QStringList AAnalyserBuild::getDriverProtocols(const QString &drvUid)
{
    //! Надо хардкодить все драйвера
    if (drvUid == DevicesDefines::uid_bedsidescales01)
        return BedsideScales01::getProtocols();
    return QStringList();
}

bool AAnalyserBuild::editDriverParams(const QString &drvUid, QJsonObject &params)
{
    //! Надо хардкодить все драйвера
    if (drvUid == DevicesDefines::uid_bedsidescales01)
        return BedsideScales01::editParams(params);
    return false;
}

Driver *AAnalyserBuild::createDriver(const QString &drvUid)
{
    //! Надо хардкодить все драйвера
    if (drvUid == DevicesDefines::uid_bedsidescales01)
        return new BedsideScales01();
    return nullptr;
}


void AAnalyserBuild::assignDrivers(QMap<QString, QString> &drivers)
{
    //! Надо хардкодить все драйвера
    drivers.insert(DevicesDefines::uid_bedsidescales01, BedsideScales01::name());
}


