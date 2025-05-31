#pragma once

namespace fre
{
	// pricing European option
	double PriceByCRR(double S0, double U, double D, double R, unsigned int N, double K);
	double HW1Function(int N, int n);
	// computing call payoff
	double CallPayoff(double z, double K);
}