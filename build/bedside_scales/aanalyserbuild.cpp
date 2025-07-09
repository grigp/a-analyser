#include "aanalyserbuild.h"

#include "aanalyserapplication.h"
#include "connection.h"

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
#include "motionrecognition.h"

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
    app->registerSignalTransformer(new MotionRecognition());
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

bool AAnalyserBuild::drvInitialSetup(const QString& uid,
                                     DeviceProtocols::Ports port,
                                     const QJsonObject &params,
                                     const QString &comment)
{
    Q_UNUSED(uid);
    Q_UNUSED(port);
    Q_UNUSED(params);
    Q_UNUSED(comment);
    return true;  //! По умолчанию true, ибо, если устройства нет, то настраивать нечего, значит удачно
}

bool AAnalyserBuild::isInitialSetup(const QString &drvUid)
{
    return false; // || (drvUid == .....::uid());
}

bool AAnalyserBuild::isAutoRunInitialSetup()
{
    return true;
}

void AAnalyserBuild::assignDrivers(QMap<QString, QString> &drivers)
{
    //! Надо хардкодить все драйвера
    drivers.insert(DevicesDefines::uid_bedsidescales01, BedsideScales01::name());
}



QStringList AAnalyserBuild::getDefaultPPList()
{
    return QStringList();
}

bool AAnalyserBuild::isPPEnabled()
{
    return false;
}

QString AAnalyserBuild::mainWindowTitle()
{
    return QCoreApplication::tr("А-Мед. Прикроватные весы");
}
