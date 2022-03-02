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
#include "trenresultfactors.h"
#include "crossfactors.h"
#include "stepoffsetfactors.h"
#include "evolventafactors.h"
#include "octaedronfactors.h"
#include "boxerdodgingmultifactor.h"
#include "stepdeviationfactors.h"
#include "trianglefactors.h"
#include "triangleconslutionfactors.h"

#include "skgvisual.h"
#include "stabilogramvisual.h"
#include "balistogramvisual.h"
#include "vectoranalysisvisual.h"

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
    TrenResultFactors::registerFactors();
    CrossFactors::registerFactors();
    StepOffsetFactors::registerFactors();
    EvolventaFactors::registerFactors();
    OctaedronFactors::registerFactors();
    BoxerDodgingMultifactor::registerFactors();
    StepDeviationFactors::registerFactors();
    TriangleFactors::registerFactors();
    TriangleConslutionFactors::registerFactors();
}

void AAnalyserBuild::registerVisuals()
{
    auto* app = static_cast<AAnalyserApplication*>(QApplication::instance());
    app->registerVisual(new SKGVisual(VisualDefines::vlChannel));
    app->registerVisual(new StabilogramVisual(VisualDefines::vlChannel));
    app->registerVisual(new BalistogramVisual(VisualDefines::vlChannel));
    app->registerVisual(new VectorAnalysisVisual(VisualDefines::vlChannel));
}
