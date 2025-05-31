// EuropeanOption2.hpp
//
// Class that represents  solutions to European options. This is
// an implementation using basic C++ syntax only. It has been
// written for pedagogical reasons
//
// (C) Datasim Component Technology BV 2003-2006
//

// EuropeanOption.hpp
#ifndef EuropeanOption_hpp
#define EuropeanOption_hpp

#include <string>
using namespace std;

class EuropeanOption
{
private:
	// Member data
	double r;
	double sig;
	double K;
	double T;
	double U;
	double b;
	char *optType;

	// Private Gaussian functions
	double n(double x) const;
	double N(double x) const;

public:
	// Constructors and destructor
	EuropeanOption();
	EuropeanOption(const EuropeanOption &option2);
	EuropeanOption(const string &optionType);
	virtual ~EuropeanOption();

	EuropeanOption &operator=(const EuropeanOption &option2);

	// 'Kernel' functions for option calculations
	double CallPrice() const;
	double PutPrice() const;
	double CallDelta() const;
	double PutDelta() const;

	// Functions that calculate option price and sensitivities
	double Price() const;
	double Delta() const;

	// Modifier functions
	void toggle(); // Change option type (C/P, P/C)

	// Getter for optType
	const char *GetOptType() const;

	// Setter functions for private members
	void SetU(double newU);
	void SetK(double newK);
	void SetT(double newT);
	void SetR(double newR);
	void SetSig(double newSig);
	void SetB(double newB);

	// Getter functions (optional)
	double GetR() const;
};

#endif
