#include <iostream>
#include <cmath>
using namespace std;

class DefInt
{
private:
    double a, b;
    double (*f)(double);

public:
    DefInt(double lower, double upper, double (*func)(double))
        : a(lower), b(upper), f(func) {}

    double ByTrapezoid(int N)
    {
        double h = (b - a) / N;
        double sum = 0.5 * (f(a) + f(b));
        for (int k = 1; k < N; ++k)
        {
            double x_k = a + k * h;
            sum += f(x_k);
        }
        return sum * h;
    }

    double BySimpson(int N)
    {
        if (N % 2 == 1)
            N++;
        double h = (b - a) / N;
        double sum = f(a) + f(b);

        for (int k = 1; k < N; k += 2)
        {
            sum += 4 * f(a + k * h);
        }
        for (int k = 2; k < N - 1; k += 2)
        {
            sum += 2 * f(a + k * h);
        }
        return sum * h / 3.0;
    }
};

double myFunction(double x)
{
    return x * x * x - x * x + 1;
}

int main()
{

    DefInt myInt(1, 2, myFunction);

    int N = 250;
    cout << "Trapezoidal Rule Approximation: " << myInt.ByTrapezoid(N) << endl;
    cout << "Simpson's Rule Approximation: " << myInt.BySimpson(N) << endl;

    return 0;
}

/*
Trapezoidal Rule Approximation: 2.41668
Simpson's Rule Approximation: 2.41667
*/
