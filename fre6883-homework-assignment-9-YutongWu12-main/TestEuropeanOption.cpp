// TestEuropeanOption.cpp
// Test program for the exact solutions of European options.
// (C) Datasim Component Technology BV 2003-2006
//

#include "EuropeanOption.hpp"
#include <iostream>
#include <cmath>
using namespace std;

int main()
{
	// Q1: Call option on a stock
	EuropeanOption callOption;
	cout << callOption.GetOptType() << " option on a stock: " << callOption.Price() << endl;
	cout << "Delta on a " << callOption.GetOptType() << " option: " << callOption.Delta() << endl;

	// Q2: Three-month European put option without dividend
	EuropeanOption putOption("P");
	putOption.SetU(50.0);
	putOption.SetK(50.0);
	putOption.SetT(0.25); // 3 months
	putOption.SetR(0.10);
	putOption.SetSig(0.30);
	putOption.SetB(putOption.GetR());

	cout << putOption.GetOptType() << " option on a stock: " << putOption.Price() << endl;

	// Q3: Three-month European put option with dividend
	EuropeanOption putDivOption("P");
	putDivOption.SetU(50.0 - 1.50 * exp(-0.10 * (2.0 / 12.0)));
	putDivOption.SetK(50.0);
	putDivOption.SetT(0.25);
	putDivOption.SetR(0.10);
	putDivOption.SetSig(0.30);
	putDivOption.SetB(putDivOption.GetR());

	cout << putDivOption.GetOptType() << " option with dividend: " << putDivOption.Price() << endl;

	return 0;
}
/*
C option on a stock: 2.13293
Delta on a C option: 0.372492
P option on a stock: 2.37561
P option with dividend: 3.03039*/