#include "generalcoordfactors.h"

#include <QDebug>

#include "datadefines.h"
#include "dataprovider.h"

#include "aanalyserapplication.h"
#include "generalcoorddefines.h"
#include "evolventafactors.h"
#include "stepdeviationfactors.h"
#include "stepoffsetfactors.h"
#include "trianglefactors.h"
#include "triangleconslutionfactors.h"

GeneralCoordFactors::GeneralCoordFactors(const QStringList &testUids, QObject *parent)
    : BaseMultifactor (testUids, parent)
{
    if (isValid())
        calculate();
}

QString GeneralCoordFactors::uid() const
{
    return GeneralCoordFactorsDefines::GroupUid;
}

QString GeneralCoordFactors::name() const
{
    return GeneralCoordFactorsDefines::GroupName;
}

bool GeneralCoordFactors::isValid() const
{
    return isValid(testUids());
}

bool GeneralCoordFactors::isValid(const QStringList &testUids)
{
    bool isStepDeviation = false;
    bool isStepOffset = false;
    bool isEvolventa = false;
    bool isTriangle = false;
    foreach (auto uid, testUids)
    {
        DataDefines::TestInfo ti;
        if (DataProvider::getTestInfo(uid, ti))
        {
            if (ti.probes.size() == 1 )
            {
                if (ti.metodUid == GeneralCoordDefines::StepDeviationTestUid)
                {
                    if (!isStepDeviation)
                        isStepDeviation = true;
                }
                else
                if (ti.metodUid == GeneralCoordDefines::StepOffsetTestUid)
                {
                    if (!isStepOffset)
                        isStepOffset = true;
                }
                else
                if (ti.metodUid == GeneralCoordDefines::EvolventaTestUid)
                {
                    if (!isEvolventa)
                        isEvolventa = true;
                }
                else
                if (ti.metodUid == GeneralCoordDefines::TriangleTestUid)
                {
                    if (!isTriangle)
                        isTriangle = true;
                }
            }
        }
    }

    return isStepDeviation && isStepOffset && isEvolventa && isTriangle;
}

void GeneralCoordFactors::calculate()
{
    assignMultifactors(testUids());
    doCalculate();
    addFactors();
}

void GeneralCoordFactors::registerFactors()
{
    GeneralCoordCalculator::registerFactors();
}

void GeneralCoordFactors::addFct(const QString &uid, const double value)
{
    addFactor(uid, value);
}

void GeneralCoordFactors::assignMultifactors(const QStringList &testUids)
{
    foreach (auto uid, testUids)
    {
        DataDefines::TestInfo ti;
        if (DataProvider::getTestInfo(uid, ti))
        {
            if (ti.probes.size() == 1 )
            {
                auto uidProbe = ti.probes.at(0);
                if (ti.metodUid == GeneralCoordDefines::StepDeviationTestUid)
                {
                    if (!stepDeviationFactors())
                        setFGStepDeviation(new StepDeviationFactors(uid, uidProbe));
                }
                else
                if (ti.metodUid == GeneralCoordDefines::StepOffsetTestUid)
                {
                    if (!stepOffsetFactors())
                        setFGStepOffset(new StepOffsetFactors(uid, uidProbe));
                }
                else
                if (ti.metodUid == GeneralCoordDefines::EvolventaTestUid)
                {
                    if (!evolventaFactors())
                        setFGEvolventa(new EvolventaFactors(uid, uidProbe));
                }
                else
                if (ti.metodUid == GeneralCoordDefines::TriangleTestUid)
                {
                    if (!triangleFactors())
                        setFGTriangle(new TriangleFactors(uid, uidProbe));
                    if (!triangleConslFactors())
                        setFGTriangleConsl(new TriangleConslutionFactors(uid, uidProbe));
                }
            }
        }
    }
}
