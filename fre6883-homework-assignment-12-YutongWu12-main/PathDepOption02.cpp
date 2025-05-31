#include "PathDepOption02.h"
#include <cmath>
#include <stdexcept> // for std::runtime_error

namespace fre
{

    PathDepOption::~PathDepOption() {}

    double PathDepOption::PriceByMC(MCModel &Model, long N)
    {
        double H = 0.0;
        SamplePath S(m);

        for (long i = 0; i < N; i++)
        {
            Model.GenerateSamplePath(T, m, S);
            H = (i * H + Payoff(S)) / (i + 1.0);
        }
        Price = std::exp(-Model.GetR() * T) * H;
        return Price;
    }

    double PathDepOption::PriceByMC(MCModel &Model, long N, double epsilon, Vector &delta)
    {
        throw std::runtime_error("Base class PriceByMC with delta not implemented.");
    }

    double ArthmAsianCall::Payoff(const SamplePath &S) const
    {
        double Ave = 0.0;
        int d = S[0].size();
        Vector one(d);
        for (int i = 0; i < d; i++)
            one[i] = 1.0;

        for (int k = 0; k < m; k++)
        {
            Ave = (k * Ave + (one ^ S[k])) / (k + 1.0);
        }
        return std::max(Ave - K, 0.0);
    }

    double ArthmAsianCall::PriceByMC(MCModel &Model, long N, double epsilon, Vector &delta)
    {
        int d = Model.GetS0().size();
        SamplePath S(m);
        std::vector<SamplePath> Paths(N, S);

        double H = 0.0;
        for (long i = 0; i < N; i++)
        {
            Model.GenerateSamplePath(T, m, Paths[i]);
            H += Payoff(Paths[i]);
        }
        Price = std::exp(-Model.GetR() * T) * (H / N);

        delta.resize(d);
        for (int j = 0; j < d; j++)
        {
            double H_eps = 0.0;
            for (long i = 0; i < N; i++)
            {
                SamplePath S_eps = Paths[i];
                for (int k = 0; k < m; k++)
                    S_eps[k][j] *= (1.0 + epsilon);
                H_eps += Payoff(S_eps);
            }
            double bumped_price = std::exp(-Model.GetR() * T) * (H_eps / N);
            delta[j] = (bumped_price - Price) / (epsilon * Model.GetS0()[j]);
        }

        return Price;
    }
}
