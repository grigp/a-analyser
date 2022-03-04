#include "aanalyserbuild.h"

#include "aanalyserapplication.h"

#include "metodictemplate.h"
#include "bedsidescalestestertemplate.h"

#include "stabilan01.h"
#include "jumpplate.h"

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
//    auto* app = static_cast<AAnalyserApplication*>(QApplication::instance());
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
