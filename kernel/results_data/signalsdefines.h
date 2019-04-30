#ifndef SIGNALSDEFINES_H
#define SIGNALSDEFINES_H

#include <QObject>

namespace SignalsDefines
{

struct StabRec
{
    double x;
    double y;
    StabRec() {}
    StabRec(std::tuple<double, double> params)
        : x(std::get<0>(params))
        , y(std::get<1>(params))
    {}
};

}

#endif // SIGNALSDEFINES_H
