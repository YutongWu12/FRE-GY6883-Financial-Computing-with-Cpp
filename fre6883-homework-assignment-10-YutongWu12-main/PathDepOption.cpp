#include "PathDepOption.h"
#include <cmath>

namespace fre
{
    void Rescale(SamplePath &S, double x)
    {
        int m = S.size();
        for (int j = 0; j < m; j++)
            S[j] = x * S[j];
    }

    double PathDepOption::PriceByMC(const MCModel &Model, long N, double epsilon)
    {
        double H = 0.0, Hsq = 0.0, Heps = 0.0, Hdown = 0.0;
        SamplePath S(m);
        for (long i = 0; i < N; i++)
        {
            // Regular simulation
            Model.GenerateSamplePath(T, m, S);
            double payoff0 = Payoff(S);
            H = (i * H + payoff0) / (i + 1.0);
            Hsq = (i * Hsq + payoff0 * payoff0) / (i + 1.0);

            // Up-shifted path
            Rescale(S, (1.0 + epsilon));
            double payoffUp = Payoff(S);
            Heps = (i * Heps + payoffUp) / (i + 1.0);

            // Down-shifted path
            Rescale(S, (1.0 - epsilon) / (1.0 + epsilon));
            double payoffDown = Payoff(S);
            Hdown = (i * Hdown + payoffDown) / (i + 1.0);
        }
        double discount = exp(-Model.GetR() * T);
        Price = discount * H;
        PricingError = discount * sqrt(Hsq - H * H) / sqrt(N - 1.0);
        delta = discount * (Heps - H) / (Model.GetS0() * epsilon);
        gamma = discount * (Heps - 2.0 * H + Hdown) / (pow(Model.GetS0() * epsilon, 2));
        return Price;
    }

    double ArthmAsianCall::Payoff(const SamplePath &S) const
    {
        double Ave = 0.0;
        for (int k = 0; k < m; k++)
            Ave = (k * Ave + S[k]) / (k + 1.0);
        if (Ave < K)
            return 0.0;
        return Ave - K;
    }
}
