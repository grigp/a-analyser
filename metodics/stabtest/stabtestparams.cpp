#include "stabtestparams.h"

StabTestParams::StabTestParams::StabTestParams(const QJsonObject params)
{
    condition = params["condition"].toInt();
    auto arrProbes = params["probes"].toArray();
    for (int i = 0; i < arrProbes.size(); ++i)
    {
        auto prbObj = arrProbes.at(i).toObject();
        ProbeParams prbParams;
        prbParams.autoEnd = prbObj["autoend"].toBool();
        prbParams.probeKind = prbObj["kind"].toInt();
        prbParams.latentTime = prbObj["latent_time"].toInt();
        prbParams.name = prbObj["name"].toString();
        prbParams.scale = prbObj["scale"].toInt();
        prbParams.stimulCode = prbObj["stimul"].toInt();
        prbParams.time = prbObj["time"].toInt();
        prbParams.zeroingEnabled = prbObj["zeroing"].toBool();

        probes << prbParams;
    }
}
