#include <iostream>
#include "PathDepOption.h"
#include "GmtrAsianCall.h"
#include <iomanip>

using namespace std;
using namespace fre;

int main()
{
    double S0 = 100.0, r = 0.03, sigma = 0.2;
    MCModel Model(S0, r, sigma);

    double T = 1.0 / 12.0, K = 100.0;
    int m = 30;

    ArthmAsianCall Option(T, K, m);
    GmtrAsianCall CVOption(T, K, m);

    long N = 30000;
    double epsilon = 0.001;

    // Pricing with variance reduction
    Option.PriceByVarRedMC(Model, N, CVOption, epsilon);
    cout << fixed << setprecision(6);
    cout << "Arithmetic call price = " << Option.GetPrice() << endl;
    cout << "Error = " << Option.GetPricingError() << endl;
    cout << "delta = " << Option.GetDelta() << endl
         << endl;

    // Pricing by direct MC
    Option.PriceByMC(Model, N, epsilon);
    cout << "Price by direct MC = " << Option.GetPrice() << endl;
    cout << "Error = " << Option.GetPricingError() << endl;
    cout << "delta = " << Option.GetDelta() << endl;

    return 0;
}

/*
Arithmetic call price = 1.425931
Error = 0.000138
delta = 0.519950

Price by direct MC = 1.415059
Error = 0.011988
delta = 0.523036
*/