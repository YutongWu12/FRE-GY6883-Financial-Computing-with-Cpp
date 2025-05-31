#include "ButterflyOpt.h"

namespace fre
{
    double ButterflyOpt::Payoff(double z) const
    {
        int Km = (K1 + K2) / 2;
        if (z > K1 && z <= Km)
            return z - K1;
        if (z > Km && z <= K2)
            return K2 - z;
        return 0.0;
    }
}