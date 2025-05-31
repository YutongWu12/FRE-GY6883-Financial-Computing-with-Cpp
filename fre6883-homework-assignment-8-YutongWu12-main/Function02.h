#pragma once
namespace fre {
    class Function
    {
    public:
        virtual double Value(double x) = 0;
        virtual double Deriv(double x) = 0;
        virtual ~Function() {}
    };
    class F1 : public Function
    {
    public:
        double Value(double x);
        double Deriv(double x);
    };

    class F2 : public Function
    {
    private:
        double a; //any real number
    public:
        F2(double a_) : a(a_) { }
        double Value(double x);
        double Deriv(double x);
    };

    class Bond : public Function
    {
    private:
        double P;
        double C1;
        double C2;
        double C3;
        double T;
        double F;
    public:
        Bond(double P_, double C1_, double C2_, double C3_, double T_, double F_) : P(P_), C1(C1_), C2(C2_), C3(C3_), T(T_), F(F_){ }
        double Value(double x);
        double Deriv(double x);
    };
}   