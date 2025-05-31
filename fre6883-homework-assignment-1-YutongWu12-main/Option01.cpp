#include "Option01.h"
#include "BinomialTreeModel.h"
#include <iostream>
#include <cmath>
using namespace std;

namespace fre
{
    double HW1Function(int N, int n)
    {
        if (n < 0 || n > N)
            return 0;
        double result = 1;
        for (int i = 1; i <= n; i++)
            result = result * (N - n + i) / i;
        return result;
    }

    double PriceByCRR(double S0, double U, double D, double R, unsigned int N, double K)
    {
        double q = RiskNeutProb(U, D, R);
        double Price = 0;
        for (int i = 0; i <= N; i++)
        {
            Price = Price + HW1Function(N, i) * pow(q, i) * pow(1 - q, N - i) * CallPayoff(CalculateAssetPrice(S0, U, D, N, i), K) / pow(R, N);
        }
        return Price;
    }

    double CallPayoff(double z, double K)
    {
        if (z > K)
            return z - K;
        return 0.0;
    }
}
