#include "aanalyserbuild.h"

#include "metodictemplate.h"
#include "stabtesttemplate.h"

#include "classicfactors.h"
#include "vectorfactors.h"
#include "ratioprobesfactors.h"
#include "idsfactors.h"
#include "targetfactors.h"

QList<MetodicTemplate *> AAnalyserBuild::getBuildTemplates(QObject *parent)
{
    return QList<MetodicTemplate *>()
            << new StabTestTemplate(parent);
}

void AAnalyserBuild::registerFactors()
{
    ClassicFactors::registerFactors();
    VectorFactors::registerFactors();
    RatioProbesFactors::registerFactors();
    IDSFactors::registerFactors();
    TargetFactors::registerFactors();
}
