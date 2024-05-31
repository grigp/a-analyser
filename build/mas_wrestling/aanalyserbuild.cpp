#include "aanalyserbuild.h"

#include "aanalyserapplication.h"

#include "metodictemplate.h"
#include "mwcompetitivesticktemplate.h"
#include "connection.h"

#include "mwstickforcefactorsdescriptor.h"

#include "dynamosignalvisual.h"
#include "sectiongraphvisual.h"

#include "commonppvisual.h"

#include "filtersignal.h"

#include "mwstick.h"

QList<MetodicTemplate *> AAnalyserBuild::getBuildTemplates(QObject *parent)
{
    return QList<MetodicTemplate *>()
            << new MWCompetitiveStickTemplate(parent);
}

void AAnalyserBuild::registerFactors()
{
    auto* app = static_cast<AAnalyserApplication*>(QApplication::instance());
    app->registerGroup(new MWStickForceFactorsDescriptor(BaseDefines::tlChannel));
}

void AAnalyserBuild::registerVisuals()
{
    auto* app = static_cast<AAnalyserApplication*>(QApplication::instance());
    app->registerVisual(new DynamoSignalVisual(BaseDefines::tlChannel));
    app->registerVisual(new SectionGraphVisual(BaseDefines::tlSection));
}

void AAnalyserBuild::registerPPVisuals()
{
//    auto* app = static_cast<AAnalyserApplication*>(QApplication::instance());
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
    if (drvUid == MWStick::uid())
        return MWStick::getPorts();

    return QList<DeviceProtocols::Ports>();
}

QStringList AAnalyserBuild::getDriverProtocols(const QString &drvUid)
{
    //! Надо хардкодить все драйвера
    if (drvUid == MWStick::uid())
        return MWStick::getProtocols();
    return QStringList();
}

bool AAnalyserBuild::editDriverParams(const QString &drvUid, QJsonObject &params)
{
    //! Надо хардкодить все драйвера
    if (drvUid == MWStick::uid())
        return MWStick::editParams(params);
    return false;
}

Driver *AAnalyserBuild::createDriver(const QString &drvUid)
{
    //! Надо хардкодить все драйвера
    if (drvUid == MWStick::uid())
        return new MWStick();
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
    Q_UNUSED(drvUid);
    return false; // || (drvUid == .....::uid());
}

bool AAnalyserBuild::isAutoRunInitialSetup()
{
    return true;
}

void AAnalyserBuild::assignDrivers(QMap<QString, QString> &drivers)
{
    //! Надо хардкодить все драйвера
    drivers.insert(MWStick::uid(), MWStick::name());
}

QStringList AAnalyserBuild::getDefaultPPList()
{
    return QStringList();
}

bool AAnalyserBuild::isPPEnabled()
{
    return false;
}

