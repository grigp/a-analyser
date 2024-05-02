#include "personalprogramservice.h"

#include "dataprovider.h"

double PersonalProgramService::getSuccessFactorValue(const QString &uidTest)
{
    auto factors = DataProvider::getPrimaryFactors(uidTest);
    foreach (auto factor, factors)
        if (factor.uid() == FactorsDefines::CommonFactors::SuccessUid)
            return factor.value();
    return -1;
}
