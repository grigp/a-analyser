#include "aanalyserbuild.h"

#include "metodictemplate.h"
#include "stabtesttemplate.h"
#include "jumptesttemplate.h"
#include "jumpheighttesttemplate.h"
#include "teppingtesttemplate.h"
#include "droptesttemplate.h"

#include "classicfactors.h"
#include "vectorfactors.h"
#include "ratioprobesfactors.h"
#include "jumpheightsinglefactors.h"
#include "jumpheightfactors.h"
#include "teppingtestfactors.h"
#include "droptestfactors.h"
#include "hoppingfactors.h"
#include "idsfactors.h"
#include "targetfactors.h"

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
    ClassicFactors::registerFactors();
    VectorFactors::registerFactors();
    RatioProbesFactors::registerFactors();
    JumpHeightSingleFactors::registerFactors();
    JumpHeightFactors::registerFactors();
    TeppingTestFactors::registerFactors();
    DropTestFactors::registerFactors();
    HoppingFactors::registerFactors();
    IDSFactors::registerFactors();
    TargetFactors::registerFactors();
}
