#include "aanalyserbuild.h"

#include "aanalyserapplication.h"

#include "metodictemplate.h"
#include "bedsidescalestestertemplate.h"

#include "devicesdefines.h"
#include "driver.h"
#include "bedsidescales01.h"

#include "balistogramvisual.h"
#include "weightplategraphvisual.h"

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
    app->registerVisual(new BalistogramVisual(VisualDefines::vlChannel));
    app->registerVisual(new WeightPlateGraphVisual(VisualDefines::vlChannel));
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


