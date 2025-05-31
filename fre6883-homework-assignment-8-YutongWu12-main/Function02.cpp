#include "Function02.h"
#include <cmath>
namespace fre {
    double F1::Value(double x) { return x * x - 2; }
    double F1::Deriv(double x) { return 2 * x; }
    
    double F2::Value(double x) { return x * x - a; }
    double F2::Deriv(double x) { return 2 * x; }

    double Bond::Value(double x) { return C1 * exp(-x*1) + C2 * exp(-x*2) + C3 * exp(-x*T) + F * exp(-x*T) - P;}
    double Bond::Deriv(double x) { return -C1 * exp(-x*1) - 2*C2 * exp(-x*2) - T*C3 * exp(-x*T) - T*F * exp(-x*T); }
}