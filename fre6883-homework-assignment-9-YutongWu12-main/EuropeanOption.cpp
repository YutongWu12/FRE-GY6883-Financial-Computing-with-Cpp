// EurpeanOption.cpp
//
//	Author: Daniel Duffy
//
// (C) Datasim Component Technology BV 2003
//

#ifndef EuropeanOption_cpp
#define EuropeanOption_cpp

#include "EuropeanOption.hpp"
#include <math.h> // math.h not cmath
#include <iostream>
#include <cstring> // for strcpy(), strcmp()
using namespace std;

//////////// Gaussian functions /////////////////////////////////

double EuropeanOption::n(double x) const
{
	double A = 1.0 / sqrt(2.0 * 3.1415);
	return A * exp(-x * x * 0.5);
}

double EuropeanOption::N(double x) const
{
	double a1 = 0.4361836;
	double a2 = -0.1201676;
	double a3 = 0.9372980;
	double k = 1.0 / (1.0 + (0.33267 * x));

	if (x >= 0.0)
	{
		return 1.0 - n(x) * (a1 * k + a2 * k * k + a3 * k * k * k);
	}
	else
	{
		return 1.0 - N(-x);
	}
}

double EuropeanOption::CallPrice() const
{
	double tmp = sig * sqrt(T);
	double d1 = (log(U / K) + (b + (sig * sig) * 0.5) * T) / tmp;
	double d2 = d1 - tmp;

	return (U * exp((b - r) * T) * N(d1)) - (K * exp(-r * T) * N(d2));
}

double EuropeanOption::PutPrice() const
{
	double tmp = sig * sqrt(T);
	double d1 = (log(U / K) + (b + (sig * sig) * 0.5) * T) / tmp;
	double d2 = d1 - tmp;

	return (K * exp(-r * T) * N(-d2)) - (U * exp((b - r) * T) * N(-d1));
}

double EuropeanOption::CallDelta() const
{
	double tmp = sig * sqrt(T);
	double d1 = (log(U / K) + (b + (sig * sig) * 0.5) * T) / tmp;
	return exp((b - r) * T) * N(d1);
}

double EuropeanOption::PutDelta() const
{
	double tmp = sig * sqrt(T);
	double d1 = (log(U / K) + (b + (sig * sig) * 0.5) * T) / tmp;
	return exp((b - r) * T) * (N(d1) - 1.0);
}

/////////////////////////////////////////////////////////////////////////////////////

EuropeanOption::EuropeanOption()
{
	r = 0.08;
	sig = 0.30;
	K = 65.0;
	T = 0.25;
	U = 60.0;
	b = r;
	optType = new char[2];
	strcpy(optType, "C");
}

EuropeanOption::EuropeanOption(const EuropeanOption &o2)
{
	r = o2.r;
	sig = o2.sig;
	K = o2.K;
	T = o2.T;
	U = o2.U;
	b = o2.b;
	optType = new char[2];
	strcpy(optType, o2.optType);
}

EuropeanOption::EuropeanOption(const string &optionType)
{
	r = 0.08;
	sig = 0.30;
	K = 65.0;
	T = 0.25;
	U = 60.0;
	b = r;
	optType = new char[2];
	if (optionType == "c" || optionType == "C")
		strcpy(optType, "C");
	else
		strcpy(optType, "P");
}

EuropeanOption::~EuropeanOption()
{
	delete[] optType;
}

EuropeanOption &EuropeanOption::operator=(const EuropeanOption &o2)
{
	if (this == &o2)
		return *this;

	r = o2.r;
	sig = o2.sig;
	K = o2.K;
	T = o2.T;
	U = o2.U;
	b = o2.b;

	delete[] optType;
	optType = new char[2];
	strcpy(optType, o2.optType);

	return *this;
}

double EuropeanOption::Price() const
{
	if (strcmp(optType, "C") == 0)
		return CallPrice();
	else
		return PutPrice();
}

double EuropeanOption::Delta() const
{
	if (strcmp(optType, "C") == 0)
		return CallDelta();
	else
		return PutDelta();
}

void EuropeanOption::toggle()
{
	if (strcmp(optType, "C") == 0)
		strcpy(optType, "P");
	else
		strcpy(optType, "C");
}

const char *EuropeanOption::GetOptType() const
{
	return optType;
}

// Setter functions
void EuropeanOption::SetU(double newU) { U = newU; }
void EuropeanOption::SetK(double newK) { K = newK; }
void EuropeanOption::SetT(double newT) { T = newT; }
void EuropeanOption::SetR(double newR) { r = newR; }
void EuropeanOption::SetSig(double newSig) { sig = newSig; }
void EuropeanOption::SetB(double newB) { b = newB; }

// Getter functions
double EuropeanOption::GetR() const { return r; }

#endif
