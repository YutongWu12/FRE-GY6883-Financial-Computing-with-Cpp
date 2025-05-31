#pragma once
#include "BinomialTreeModel02.h"
#include <cmath>

namespace fre
{
    class BSModel
    {
    private:
        double r;     // interest rate
        double h;     // time step
        double sigma; // volatility
        double S0;    // initial stock price

    public:
        BSModel() : r(0), h(0), sigma(0), S0(0) {}
        BSModel(double S0_, double r_, double h_, double sigma_) : S0(S0_), r(r_), h(h_), sigma(sigma_) {}
        ~BSModel() {}

        BinomialTreeModel generateModel() const
        {
            double U = exp(sigma * sqrt(h));
            double D = 1.0 / U;
            double R = exp(r * h);
            return BinomialTreeModel(S0, U, D, R);
        }
    };
}
